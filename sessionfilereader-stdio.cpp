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
			 NORMAL,DATA_BEFORE,DATA,DATA_AFTER,WHITESPACE
			};
		enum class TokenType:uint8_t{TITLE,SECTION_TITLE,KEY,VALUE,INVALID};

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
	,m_state{State::NORMAL},m_state_old{State::NORMAL}
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
			case TokenType::TITLE:
				record.sectionTitleSet(m_tok.buffer);
				record.sectionLevelSet(0);
				break;

			case TokenType::SECTION_TITLE:
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
	while(m_tok.type!=TokenType::SECTION_TITLE
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
	auto state=m_state;
	auto state_old=m_state_old;
	auto token_type=m_token_type;
	tok.buffer.clear();
	while( (ch_in=getc(fptr)) != EOF)
		{
		switch(state)
			{
			case State::WHITESPACE:
				if(ch_in=='\n' && tok.buffer.length()!=0)
					{
					switch(token_type)
						{
						case TokenType::VALUE:
							tok.buffer.truncate();
							tok.buffer.append('\0');
							tok.type=token_type;
							m_token_type=TokenType::INVALID;
							m_state_old=State::NORMAL;
							m_state=state;
							return 1;

						default:
							break;
						}
					break;
					}
				else
				if(!(ch_in>=0 && ch_in<=' '))
					{
					if(state_old!=State::NORMAL)
						{tok.buffer.append(ch_in);}
					state=state_old;
					}
				else
					{break;}

			case State::NORMAL:
				if(token_type==TokenType::INVALID)
					{
					switch(ch_in)
						{
						case '=':
							state=State::DATA_BEFORE;
							token_type=TokenType::TITLE;
							break;

						case '-':
							state=State::DATA_BEFORE;
							token_type=TokenType::SECTION_TITLE;
							break;

						default:
							state=State::DATA;
							token_type=TokenType::KEY;
							tok.buffer.append(ch_in);
							break;
						}
					}
				break;

			case State::DATA_BEFORE:
				switch(token_type)
					{
					case TokenType::TITLE:
						if(ch_in!='=')
							{
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							else
								{
								state=State::DATA;
								tok.buffer.append(ch_in);
								}
							}
						break;

					case TokenType::SECTION_TITLE:
						if(ch_in!='-')
							{
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							else
								{
								state=State::DATA;
								tok.buffer.append(ch_in);
								}
							}
						break;

					default:
						break;
					}
				break;

			case State::DATA:
				switch(token_type)
					{
					case TokenType::TITLE:
						if(ch_in=='=')
							{
							tok.type=token_type;
							tok.buffer.append('\0');
							m_state=State::DATA_AFTER;
							m_token_type=token_type;
							return 1;
							}
						else
							{
							tok.buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							}
						break;

					case TokenType::KEY:
						if(ch_in==':')
							{
							tok.type=token_type;
							tok.buffer.append('\0');
							m_state=State::DATA_AFTER;
							m_token_type=token_type;
							return 1;
							}
						else
							{
							tok.buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							}
						break;

					case TokenType::VALUE:
						if(ch_in=='\n')
							{
							tok.buffer.append(' ');
							state=State::WHITESPACE;
							state_old=State::DATA;
							}
						else
							{
							tok.buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							}

						break;

					case TokenType::SECTION_TITLE:
						if(ch_in=='-')
							{
							tok.type=token_type;
							tok.buffer.append('\0');
							m_state=State::DATA_AFTER;
							m_token_type=token_type;
							return 1;
							}
						else
							{
							tok.buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							}
						break;

					default:
						break;
					}
				break;

			case State::DATA_AFTER:
				switch(token_type)
					{
					case TokenType::TITLE:
						if(ch_in!='=')
							{
							if(ch_in=='\n')
								{
								state=State::WHITESPACE;
								state_old=State::NORMAL;
								token_type=TokenType::INVALID;
								}
							else
								{throw "Junk after title";}
							}
						break;

					case TokenType::KEY:
						if(ch_in>=0 && ch_in<=' ')
							{
							state=State::WHITESPACE;
							state_old=State::DATA;
							token_type=TokenType::VALUE;
							}
						break;

					case TokenType::SECTION_TITLE:
						if(ch_in!='-')
							{
							if(ch_in=='\n')
								{
								state=State::WHITESPACE;
								state_old=State::NORMAL;
								token_type=TokenType::INVALID;
								}
							else
								{throw "Junk after section title";}
							}
						break;

					default:
						break;
					}
				break;
			}
		}
	tok.type=token_type;
	tok.buffer.append('\0');
	return 1;
	}
