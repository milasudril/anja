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
			,VALUE_NEWLINE
			};
		enum class TokenType:uint8_t{TITLE,SLOT_TITLE,KEY,VALUE};
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
	auto state=m_state_current;
	auto state_old=state;
	ArrayDynamicShort<char> buffer;
	auto token_type=TokenType::VALUE;

	while( (ch_in=getc(m_source)) != EOF)
		{
		switch(state)
			{
			case State::NORMAL:
				if(ch_in>=0 && ch_in<=' ')
					{
					state=State::WHITESPACE;
					state_old=State::NORMAL;
					}
				else
					{
					switch(ch_in)
						{
						case '=':
							token_type=TokenType::TITLE;
							state=State::DATA_BEFORE;
							break;

						case '-':
							token_type=TokenType::SLOT_TITLE;
							state=State::DATA_BEFORE;
							break;

						default:
							buffer.append(ch_in);
							token_type=TokenType::KEY;
							state=State::DATA;
							break;
						}
					}


				break;

			case State::WHITESPACE:
				if(!(ch_in>=0 && ch_in<=' '))
					{
					if(state_old!=State::NORMAL)
						{buffer.append(ch_in);}
					state=state_old;
					}
				break;

			case State::VALUE_NEWLINE:
				if(ch_in=='\n')
					{
					state=State::NORMAL;
					buffer.append('\0');
					printf("%u [%s]\n",token_type,buffer.begin());
					buffer.clear();
					}
				else
					{
					if(ch_in>=0 && ch_in<=' ')
						{
						state=State::WHITESPACE;
						state_old=State::DATA;
						}
					}
				break;

			case State::DATA:
				switch(token_type)
					{
					case TokenType::TITLE:
						if(ch_in=='=')
							{
							buffer.append('\0');
							state=State::DATA_AFTER;
							printf("%u [%s]\n",token_type,buffer.begin());
							buffer.clear();
							}
						else
							{
							buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state_old=state;
								state=State::WHITESPACE;
								}
							}
						break;

					case TokenType::SLOT_TITLE:
						if(ch_in=='-')
							{
							buffer.append('\0');
							state=State::DATA_AFTER;
							printf("%u [%s]\n",token_type,buffer.begin());
							buffer.clear();
							}
						else
							{
							buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state_old=state;
								state=State::WHITESPACE;
								}
							}
						break;

					case TokenType::KEY:
						if(ch_in==':')
							{
							buffer.append('\0');
							state=State::DATA_AFTER;
							printf("%u [%s]\n",token_type,buffer.begin());
							buffer.clear();
							}
						else
							{
							buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state_old=state;
								state=State::WHITESPACE;
								}
							}
						break;

					case TokenType::VALUE:
						if(ch_in=='\n')
							{state=State::VALUE_NEWLINE;}
						else
							{
							buffer.append(ch_in);
							if(ch_in>=0 && ch_in<=' ')
								{
								state_old=state;
								state=State::WHITESPACE;
								}
							}
						break;
					}
				break;

			case State::DATA_AFTER:
				switch(token_type)
					{
					case TokenType::TITLE:
					case TokenType::SLOT_TITLE:
						if(ch_in=='\n')
							{
							state_old=State::DATA;
							state=State::WHITESPACE;
							token_type=TokenType::KEY;
							}
						break;
					case TokenType::KEY:
						token_type=TokenType::VALUE;
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
						break;
					default:
						throw "Error parsing input file";
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
								buffer.append(ch_in);
								state=State::DATA;
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
								buffer.append(ch_in);
								state=State::DATA;
								}
							}
						break;

					default:
						throw "Error parsing input file";
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

