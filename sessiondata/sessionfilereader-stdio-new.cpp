//@	{"targets":[{"name":"sessionfilereader-new.o","type":"object"}]}

#include "sessionfilereader.hpp"
#include "sessionfilerecordimpl.hpp"
#include "../common/string.hpp"
#include "../common/error.hpp"
#include "../common/syserror.hpp"
#include "../common/pathutils.hpp"

#include <cstdint>
#include <cstdio>
#include <cassert>

using namespace Anja;

class SessionFileReader::Impl
	{
	public:
		explicit Impl(const char* filename);
		~Impl();

		bool recordNextGet(SessionFileRecord& record);

	private:
		std::unique_ptr<FILE,decltype(&fclose)> m_source;

		enum class State:uint8_t
			{
			 ESCAPE,INIT,SECTION_BEGIN,SECTION_TITLE,SECTION_TITLE_WHITESPACE
			,RECORD,KEY,VALUE,KEY_WHITESPACE,VALUE_WHITESPACE,VALUE_NEWLINE
			};

		State m_state;
		State m_state_old;
		bool recordGet(SessionFileRecord& tok);
	};

SessionFileReader::SessionFileReader(const char* filename)
	{m_impl.reset(new Impl(filename));}

SessionFileReader::~SessionFileReader()
	{}

SessionFileReader::Impl::Impl(const char* filename):
	m_source{fopen(filename,"rb"),fclose}
	,m_state{State::INIT},m_state_old{State::INIT}
	{
	if(m_source.get()==NULL)
		{
		throw Error("It was not possible to open the file ",filename,". ",SysError(errno));
		}

	}

SessionFileReader::Impl::~Impl()
	{}

bool SessionFileReader::check(const char* filename)
	{
	if(fileIs(filename))
		{
		try
			{
			SessionFileReader reader(filename);
			SessionFileRecordImpl rec;
			return reader.recordNextGet(rec);
			}
		catch(...)
			{return 0;}
		}
	return 0;
	}

bool SessionFileReader::recordNextGet(SessionFileRecord& record)
	{
	return m_impl->recordNextGet(record);
	}

bool SessionFileReader::Impl::recordNextGet(SessionFileRecord& record)
	{
	return recordGet(record);
	}

static constexpr bool whitespace(char ch_in) noexcept
	{
	return (ch_in>=0 && ch_in<=' ');
	}

bool SessionFileReader::Impl::recordGet(SessionFileRecord& record)
	{
	record.clear();
	auto fptr=m_source.get();
	if(feof(fptr))
		{return 0;}

	auto state_current=m_state;
	auto state_next=State::INIT;
	String buffer;
	String key;
	int ch_in=0;
	auto level_count=0;
	auto nlcount=0;

	while(true)
		{
		ch_in=getc(fptr);

		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
		//	Generic escape state
			case State::ESCAPE:
				if(ch_in==EOF)
					{throw Error("Lonely escape symbol");}
				state_current=state_next;
				buffer.append(ch_in);
				break;

		//	Initial state
			case State::INIT:
				switch(ch_in)
					{
					case '#':
						state_current=State::SECTION_BEGIN;
						break;

					default:
						if(!whitespace(ch_in))
							{throw Error("Invalid session file");}
					}
				break;

		//	States for reading section title
			case State::SECTION_BEGIN:
				switch(ch_in)
					{
					case '#':
						++level_count;
						break;

					case '\\':
						state_next=State::SECTION_TITLE;
						state_current=State::ESCAPE;
						break;

					case EOF:
					case '\n':
						throw Error("Section titles must not be empty");

					default:
						if(whitespace(ch_in))
							{state_current=State::SECTION_TITLE_WHITESPACE;}
						else
							{
							buffer.append(ch_in);
							state_current=State::SECTION_TITLE;
							}
					}
				break;

			case State::SECTION_TITLE_WHITESPACE:
				switch(ch_in)
					{
					case '\\':
						state_next=State::SECTION_TITLE;
						state_current=State::ESCAPE;
						break;

					case '\n':
						if(buffer.length()==0)
							{throw Error("Section titles must not be empty");}
						record.titleSet(buffer);
						record.levelSet(level_count);
						level_count=0;
						buffer.clear();
						state_current=State::RECORD;
						break;

					case EOF:
						if(buffer.length()==0)
							{throw Error("Section titles must not be empty");}
						record.titleSet(buffer);
						record.levelSet(level_count);
						return 1;

					default:
						if(!whitespace(ch_in))
							{
							buffer.append(ch_in);
							state_current=State::SECTION_TITLE;
							}
					}
				break;

			case State::SECTION_TITLE:
				switch(ch_in)
					{
					case '\\':
						state_next=State::SECTION_TITLE;
						state_current=State::ESCAPE;
						break;

					case '\n':
						if(buffer.length()==0)
							{throw Error("Section titles must not be empty");}
						record.titleSet(buffer);
						record.levelSet(level_count);
						level_count=0;
						buffer.clear();
						state_current=State::RECORD;
						break;

					case EOF:
						if(buffer.length()==0)
							{throw Error("Section titles must not be empty");}
						record.titleSet(buffer);
						record.levelSet(level_count);
						return 1;

					default:
						buffer.append(ch_in);
						if(whitespace(ch_in))
							{state_current=State::SECTION_TITLE_WHITESPACE;}
					}
				break;


		//	Record state
			case State::RECORD:
				switch(ch_in)
					{
					case EOF:
					case '#':
						if(buffer.length()==0)
							{
							if(key.length()!=0)
								{record.propertySet(key,buffer);}
							}
						else
							{record.propertySet(key.length()?key:String("Description"),buffer);}
						buffer.clear();
						m_state=State::SECTION_BEGIN;
						return 1;

					case '~':
						if(buffer.length()==0)
							{
							if(key.length()!=0)
								{record.propertySet(key,buffer);}
							}
						else
							{record.propertySet(key.length()?key:String("Description"),buffer);}
						buffer.clear();
						state_current=State::KEY;
						break;

					case '\\':
						state_current=State::ESCAPE;
						state_next=State::VALUE;
						break;

					default:
						if(!whitespace(ch_in))
							{
							state_current=State::VALUE;
							buffer.append(ch_in);
							}
					}
				break;

		//	States reading keys
			case State::KEY:
				switch(ch_in)
					{
					case '\\':
						state_current=State::ESCAPE;
						state_next=State::KEY;
						break;

					case ':':
					case '\n':
						key=buffer;
						buffer.clear();
						state_current=State::RECORD;
						break;

					case EOF:
						record.propertySet(buffer,String(""));
						return 1;

					default:
						buffer.append(ch_in);
						if(whitespace(ch_in))
							{state_current=State::KEY_WHITESPACE;}
					}
				break;

			case State::KEY_WHITESPACE:
				switch(ch_in)
					{
					case '\\':
						state_current=State::ESCAPE;
						state_next=State::KEY;
						break;

					case ':':
					case '\n':
						key=buffer;
						buffer.clear();
						state_current=State::RECORD;
						break;

					case EOF:
						record.propertySet(buffer,String(""));
						return 1;

					default:
						if(!whitespace(ch_in))
							{
							buffer.append(ch_in);
							state_current=State::KEY;
							}

					}
				break;


		//	States reading values

			case State::VALUE:
				switch(ch_in)
					{
					case '\\':
						state_current=State::ESCAPE;
						state_next=State::VALUE;
						break;

					case '\n':
						state_current=State::VALUE_NEWLINE;
						nlcount=1;
						break;

					case EOF:
						record.propertySet(key,buffer);
						return 1;

					default:
						if(whitespace(ch_in)) //We do not know yet if we should use this whitespace or not
							{state_current=State::VALUE_WHITESPACE;}
						else
							{buffer.append(ch_in);}
					}
				break;

			case State::VALUE_WHITESPACE:
				switch(ch_in)
					{
					case '\\':
						if(buffer.length()!=0)
							{buffer.append(' ');} //Append *one* space
						state_current=State::ESCAPE;
						state_next=State::VALUE;
						break;

					case '\n':
						state_current=State::VALUE_NEWLINE;
						nlcount=1;
						break;

					case EOF:
						record.propertySet(key.length()?key:String("Description"),buffer);
						return 1;

					default:
						if(!whitespace(ch_in))
							{
							if(buffer.length()!=0)
								{buffer.append(' ');} //Append *one* space
							buffer.append(ch_in);
							state_current=State::VALUE;
							}
					}
				break;

			case State::VALUE_NEWLINE:
				switch(ch_in)
					{
					case '\\':
						if(buffer.length()!=0)
							{
							if(nlcount>=2)
								{buffer.append("\n\n");}
							else
								{buffer.append(' ');}
							}
						state_current=State::ESCAPE;
						state_next=State::VALUE;
						break;

					case '\n':
						++nlcount;
						break;

					case '#':
						record.propertySet(key.length()?key:String("Description"),buffer);
						buffer.clear();
						m_state=State::SECTION_BEGIN;
						return 1;

					case EOF:
						record.propertySet(key.length()?key:String("Description"),buffer);
						return 1;

					case '~':
						state_current=State::KEY;
						record.propertySet(key.length()?key:String("Description"),buffer);
						buffer.clear();
						break;

					default:
						if(!whitespace(ch_in))
							{
							if(buffer.length()!=0)
								{
								if(nlcount>=2)
									{buffer.append("\n\n");}
								else
									{buffer.append(' ');}
								}
							buffer.append(ch_in);
							state_current=State::VALUE;
							}
					}
				break;
			}
		}
	}
