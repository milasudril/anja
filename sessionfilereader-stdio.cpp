#ifdef __WAND__
target[name[sessionfilereader.o] type[object]]
#endif

#include "sessionfilereader.h"
#include "sessionfilerecord.h"
#include "framework/arraydynamicshort.h"
#include <cstdint>
#include <cstdio>
#include <cassert>

class SessionFileReaderStdio:public SessionFileReader
	{
	public:
		SessionFileReaderStdio(const char* filename);
		~SessionFileReaderStdio();

		bool recordNextGet(SessionFileRecord& record);

	private:
		std::unique_ptr<FILE,decltype(&fclose)> m_source;

		enum class State:uint8_t
			{
			  INIT,SECTION_TITLE_BEFORE,SECTION_TITLE,SECTION_TITLE_AFTER
			 ,KEY,VALUE,NEWLINE,WHITESPACE
			};

		enum class TokenType:uint8_t{SECTION_TITLE_0,SECTION_TITLE_1,KEY,VALUE,INVALID};

		struct Token
			{
			ArrayDynamicShort<char> buffer;
			TokenType type;
			};

		TokenType m_token_type;
		State m_state;
		State m_state_old;
		Token m_tok;
		bool tokenGet(Token& tok);
	};

SessionFileReader* SessionFileReader::instanceCreate(const char* filename)
	{
	return new SessionFileReaderStdio(filename);
	}

void SessionFileReader::destroy(SessionFileReader* reader)
	{
	delete reinterpret_cast<SessionFileReaderStdio*>(reader);
	}

SessionFileReaderStdio::SessionFileReaderStdio(const char* filename):
	m_source{fopen(filename,"rb"),fclose},m_token_type{TokenType::INVALID}
	,m_state{State::INIT},m_state_old{State::INIT}
	{
	if(m_source.get()==NULL)
		{
	//	TODO throw something better than "const char*"
		throw "Could not open session file.";
		}
	if(!tokenGet(m_tok))
		{throw "Invalid session file.";}
	}

SessionFileReaderStdio::~SessionFileReaderStdio()
	{
	}

bool SessionFileReaderStdio::recordNextGet(SessionFileRecord& record)
	{
	record.clear();
	ArrayDynamicShort<char> key_temp;

	do
		{
		switch(m_tok.type)
			{
			case TokenType::SECTION_TITLE_0:
				record.sectionTitleSet(m_tok.buffer);
				record.sectionLevelSet(0);
				break;

			case TokenType::SECTION_TITLE_1:
				record.sectionTitleSet(m_tok.buffer);
				record.sectionLevelSet(1);
				break;

			case TokenType::KEY:
				key_temp=m_tok.buffer;
				break;

			case TokenType::VALUE:
				record.propertyReplace(key_temp,m_tok.buffer);
				break;

			default:
				return 0;
			}
		if(!tokenGet(m_tok))
			{return 1;}
		}
	while(m_tok.type!=TokenType::SECTION_TITLE_1
			&& m_tok.type!=TokenType::INVALID);
	return 1;
	}

bool SessionFileReaderStdio::tokenGet(Token& tok)
	{
	auto fptr=m_source.get();
	if(feof(fptr))
		{
		tok.type=TokenType::INVALID;
		return 0;
		}

	int ch_in;
	int ch_trigg;
	auto state_current=m_state;
	auto state_prev=State::INIT;
	tok.type=TokenType::INVALID;
	tok.buffer.clear();
	while( (ch_in=getc(fptr)) != EOF)
		{
		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
			case State::INIT:
				if(ch_in=='=' || ch_in=='-')
					{
					ch_trigg=ch_in;
					state_current=State::SECTION_TITLE_BEFORE;
					}
				else
				if(!(ch_in>='\0' && ch_in<=' '))
					{
					tok.buffer.append(ch_in);
					state_current=State::KEY;
					}
				break;

			case State::WHITESPACE:
				if(ch_in=='\n')
					{
					state_current=State::NEWLINE;
					}
				else
				if(!(ch_in>='\0' && ch_in<=' '))
					{
					state_current=state_prev;
					if(tok.buffer.length()>0)
						{tok.buffer.append(' ');}
					tok.buffer.append(ch_in);
					}
				break;

			case State::SECTION_TITLE_BEFORE:
				if(ch_in>='\0' && ch_in<=' ')
					{throw "Whitespace not allowed before section title";}

				if(ch_in!=ch_trigg)
					{
					tok.buffer.append(ch_in);
					state_current=State::SECTION_TITLE;
					}
				break;

			case State::SECTION_TITLE:
				if(ch_in=='\n')
					{
					state_prev=state_current;
					state_current=State::NEWLINE;
					}
				else
				if(ch_in>='\0' && ch_in<=' ')
					{
					state_prev=state_current;
					state_current=State::WHITESPACE;
					}
				else
				if(ch_in==ch_trigg)
					{
					state_current=State::SECTION_TITLE_AFTER;
					}
				else
					{tok.buffer.append(ch_in);}
				break;

			case State::SECTION_TITLE_AFTER:
				if(ch_in=='\n')
					{
					tok.buffer.append('\0');
					m_state=State::INIT;
					tok.type=(ch_trigg=='=')?
						TokenType::SECTION_TITLE_0:TokenType::SECTION_TITLE_1;
					return 1;
					}

				if(!(ch_in==ch_trigg || ch_in=='\n'))
					{throw "Invalid character after section title";}
				break;

			case State::KEY:
				if(ch_in==':')
					{
					tok.buffer.append('\0');
					m_state=State::VALUE;
					tok.type=TokenType::KEY;
					return 1;
					}

				if(ch_in=='\n')
					{
					state_prev=state_current;
					state_current=State::NEWLINE;
					}
				else
				if(ch_in>='\0' && ch_in<=' ')
					{
					state_prev=state_current;
					state_current=State::WHITESPACE;
					}
				else
					{tok.buffer.append(ch_in);}
				break;

			case State::VALUE:
				if(ch_in=='\n')
					{
					state_prev=state_current;
					state_current=State::NEWLINE;
					}
				else
				if(ch_in>='\0' && ch_in<=' ')
					{
					state_prev=state_current;
					state_current=State::WHITESPACE;
					}
				else
					{tok.buffer.append(ch_in);}
				break;

			case State::NEWLINE:
				if(ch_in=='\n')
					{
					if(state_prev!=State::VALUE)
						{
						throw "Section title or key cannot end with newline";
						}

					tok.buffer.append('\0');
					tok.type=TokenType::VALUE;
					m_state=State::INIT;
					return 1;
					}
				else
				if(!(ch_in>='\0' && ch_in<=' '))
					{
					tok.buffer.append(' ').append(ch_in);
					state_current=state_prev;
					}
				break;
			}
		}

	if(state_current==State::VALUE || state_prev==State::VALUE)
		{
		tok.buffer.append('\0');
		tok.type=TokenType::VALUE;

		return 1;
		}
	if(state_current!=State::INIT)
		{throw "Incomplete record";}
	return 0;
	}
