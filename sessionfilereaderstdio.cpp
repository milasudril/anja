#ifdef __WAND__
target[name[sessionfilereader.o] type[object]]
#endif

#include "sessionfilereader.h"
#include "framework/array_dynamic_short.h"
#include <cstdint>
#include <cstdio>
#include <cassert>

class SessionFileReaderStdio:public SessionFileReader
	{
	public:
		SessionFileReaderStdio(const char* filename,Session& session_data);
		~SessionFileReaderStdio();

		bool nextSlotGet(WaveformData& slot);

	private:
		FILE* m_source;

		enum class State:uint8_t
			{
			 NORMAL,DATA_BEFORE,DATA,DATA_AFTER,WHITESPACE
			};
		enum class TokenType:uint8_t{TITLE,SLOT_TITLE,KEY,VALUE,INVALID};
		ArrayDynamicShort<char> m_buffer;
		ArrayDynamicShort<char> m_title;
		State m_state_current;
		bool runToState();
	};

SessionFileReader* SessionFileReader::instanceCreate(
	const char* filename,Session& session_data)
	{
	return new SessionFileReaderStdio(filename,session_data);
	}

void SessionFileReader::destroy(SessionFileReader* reader)
	{
	delete reinterpret_cast<SessionFileReaderStdio*>(reader);
	}

SessionFileReaderStdio::SessionFileReaderStdio(const char* filename,Session& session_data)
	{
	m_source=fopen(filename,"rb");
	if(m_source==NULL)
		{
	//	TODO throw something better than "const char*"
		throw "Could not open session file";
		}

	if(!runToState())
		{throw "Invalid session file";}
	}

SessionFileReaderStdio::~SessionFileReaderStdio()
	{
	fclose(m_source);
	}

bool SessionFileReaderStdio::runToState()
	{
	int ch_in;
	auto state=State::NORMAL;
	auto state_old=state;
	ArrayDynamicShort<char> buffer;
	auto token_type=TokenType::INVALID;

	while( (ch_in=getc(m_source)) != EOF)
		{
		switch(state)
			{
			case State::WHITESPACE:
				if(ch_in=='\n' && buffer.length()!=0)
					{
					switch(token_type)
						{
						case TokenType::VALUE:
							buffer.truncate();
							buffer.append('\0');
							printf("Value: [%s]\n",buffer.begin());
							buffer.clear();
							token_type=TokenType::INVALID;
							state_old=State::NORMAL;
							break;
						default:
							break;
						}
					break;
					}
				else
				if(!(ch_in>=0 && ch_in<=' '))
					{
					if(state_old!=State::NORMAL)
						{buffer.append(ch_in);}
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
							token_type=TokenType::SLOT_TITLE;
							break;

						default:
							state=State::DATA;
							token_type=TokenType::KEY;
							buffer.append(ch_in);
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
								buffer.append(ch_in);
								}
							}
						break;

					case TokenType::SLOT_TITLE:
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
								buffer.append(ch_in);
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
							buffer.append('\0');
							printf("Title: [%s]\n",buffer.begin());
							buffer.clear();
							state=State::DATA_AFTER;
							}
						else
							{
							buffer.append(ch_in);
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
							buffer.append('\0');
							printf("Key: [%s]\n",buffer.begin());
							buffer.clear();
							state=State::DATA_AFTER;
							}
						else
							{
							buffer.append(ch_in);
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
							buffer.append(' ');
							state=State::WHITESPACE;
							state_old=State::DATA;
							}
						else
							{
							buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state=State::WHITESPACE;
								state_old=State::DATA;
								}
							}

						break;

					case TokenType::SLOT_TITLE:
						if(ch_in=='-')
							{
							buffer.append('\0');
							printf("Slot title: [%s]\n",buffer.begin());
							buffer.clear();
							state=State::DATA_AFTER;
							}
						else
							{
							buffer.append(ch_in);
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

					case TokenType::SLOT_TITLE:
						if(ch_in!='-')
							{
							if(ch_in=='\n')
								{
								state=State::WHITESPACE;
								state_old=State::NORMAL;
								token_type=TokenType::INVALID;
								}
							else
								{throw "Junk after slot title";}
							}
						break;

					default:
						break;
					}
				break;
			}
		}
	return 0;
	}

bool SessionFileReaderStdio::nextSlotGet(WaveformData& slot)
	{
	return 0;
	}

