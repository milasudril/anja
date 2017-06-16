//@	{"targets":[{"name":"sessionfilewriter.o","type":"object"}]}

#include "sessionfilewriter.hpp"
#include "sessionfilerecord.hpp"
#include "../common/string.hpp"
#include "../common/error.hpp"
#include "../common/syserror.hpp"
#include <cstdint>
#include <cstdio>
#include <cassert>

using namespace Anja;

class SessionFileWriter::Impl
	{
	public:
		explicit Impl(const char* filename);
		~Impl();

		void recordWrite(const SessionFileRecord& record);

	private:
		std::unique_ptr<FILE,decltype(&fclose)> m_sink;
	};

SessionFileWriter::SessionFileWriter(const char* filename)
	{
	m_impl.reset( new Impl(filename) );
	}
SessionFileWriter::~SessionFileWriter()
	{}

SessionFileWriter::Impl::Impl(const char* filename):
	m_sink{fopen(filename,"wb"),fclose}
	{
	if(m_sink.get()==NULL)
		{throw Error("It was not possible to open the file ",filename,". ",SysError(errno));}
	}

SessionFileWriter::Impl::~Impl()
	{}

void SessionFileWriter::recordWrite(const SessionFileRecord& record)
	{
	m_impl->recordWrite(record);
	}

static constexpr bool whitespace(char ch)
	{return ch>=0 && ch<=' ';}

static void titleWrite(const char* str,FILE* output)
	{
	enum class State:int{INIT,NORMAL,SPACE};
	auto state_current=State::INIT;
	while(1)
		{
		auto ch_in=*str;
		++str;
		if(ch_in=='\r')
			{continue;}
		if(ch_in=='\0')
			{return;}

		switch(state_current)
			{
			case State::INIT:
				switch(ch_in)
					{
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{putc('\\',output);}
						putc(ch_in,output);
						state_current=State::NORMAL;
					}
				break;

			case State::NORMAL:
				switch(ch_in)
					{
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{
							if(ch_in==' ')
								{
								putc(ch_in,output);
								state_current=State::SPACE;
								}
							else
								{
								putc('\\',output);
								putc(ch_in,output);
								}
							}
						else
							{putc(ch_in,output);}
					}
				break;


			case State::SPACE:
				switch(ch_in)
					{
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{
							putc('\\',output);
							putc(ch_in,output);
							}
						else
							{putc(ch_in,output);}
						state_current=State::NORMAL;
					}
				break;

			}
		}
	}

static void keyWrite(const char* str,FILE* output)
	{
	enum class State:int{INIT,NORMAL,SPACE};
	auto state_current=State::INIT;
	while(1)
		{
		auto ch_in=*str;
		++str;
		if(ch_in=='\r')
			{continue;}
		if(ch_in=='\0')
			{return;}

		switch(state_current)
			{
			case State::INIT:
				switch(ch_in)
					{
					case ':':
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{putc('\\',output);}
						putc(ch_in,output);
						state_current=State::NORMAL;
					}
				break;

			case State::NORMAL:
				switch(ch_in)
					{
					case ':':
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{
							if(ch_in==' ')
								{
								putc(ch_in,output);
								state_current=State::SPACE;
								}
							else
								{
								putc('\\',output);
								putc(ch_in,output);
								}
							}
						else
							{putc(ch_in,output);}
					}
				break;


			case State::SPACE:
				switch(ch_in)
					{
					case ':':
					case '\n':
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{
							putc('\\',output);
							putc(ch_in,output);
							}
						else
							{putc(ch_in,output);}
						state_current=State::NORMAL;
					}
				break;

			}
		}
	}

static void valueWrite(const char* str,FILE* output)
	{
	enum class State:int{INIT,NORMAL,SPACE,NEWLINE};
	auto state_current=State::INIT;
	auto nlcount=0;

	while(1)
		{
		auto ch_in=*str;
		++str;
		if(ch_in=='\r')
			{continue;}

		if(ch_in=='\0')
			{return;}

		switch(state_current)
			{
			case State::INIT:
				switch(ch_in)
					{
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					default:
						if(whitespace(ch_in))
							{putc('\\',output);}
						putc(ch_in,output);
						state_current=State::NORMAL;
					}
				break;

			case State::NORMAL:
				switch(ch_in)
					{
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						break;

					case '\n':
						state_current=State::NEWLINE;
						++nlcount;
						break;

					default:
						if(whitespace(ch_in))
							{
							if(ch_in==' ')
								{
								putc(ch_in,output);
								state_current=State::SPACE;
								}
							else
								{
								putc('\\',output);
								putc(ch_in,output);
								}
							}
						else
							{putc(ch_in,output);}
					}
				break;

			case State::SPACE:
				switch(ch_in)
					{
					case '\\':
						putc('\\',output);
						putc(ch_in,output);
						state_current=State::NORMAL;
						break;

					case '\n':
						state_current=State::NEWLINE;
						++nlcount;
						break;

					default:
						if(whitespace(ch_in))
							{
							putc('\\',output);
							putc(ch_in,output);
							}
						else
							{putc(ch_in,output);}
						state_current=State::NORMAL;
					}
				break;

			case State::NEWLINE:
				switch(ch_in)
					{
					case '~':
					case '#':
					case '\\':
						assert(nlcount>0 && nlcount<3);
						switch(nlcount)
							{
							case 1:
								fputs("\\\n",output);
								break;
							case 2:
								fputs("\n\n\\",output);
								break;
							}
						putc(ch_in,output);
						state_current=State::NORMAL;
						nlcount=0;
						break;

					case '\n':
						++nlcount;
						if(nlcount>=3)
							{
							fputs("\n\n\\\n",output);
							state_current=State::NORMAL;
							}
						break;

					default:
						assert(nlcount>0 && nlcount<3);
						switch(nlcount)
							{
							case 1:
								fputs("\\\n",output);
								break;
							case 2:
								fputs("\n\n",output);
								break;
							}
						if(whitespace(ch_in))
							{
							if(nlcount==2)
								{putc('\\',output);}
							putc(ch_in,output);
							state_current=State::SPACE;
							}
						else
							{
							putc(ch_in,output);
							state_current=State::NORMAL;
							}
						nlcount=0;
					}
				break;


			}
		}
	}

static bool multipar(const char* str)
	{
	auto nlcount=0;
	while(1)
		{
		auto ch_in=*str;
		switch(ch_in)
			{
			case '\0':
				return 0;
			case '\n':
				if(nlcount>=2)
					{return 1;}
				++nlcount;
				break;
			default:
				nlcount=0;
			}
		++str;
		}
	}

void SessionFileWriter::Impl::recordWrite(const SessionFileRecord& record)
	{
	//Write title
		{
		auto k=record.levelGet() + 1;
		while(k!=0)
			{
			--k;
			putc('#',m_sink.get());
			}
		titleWrite(record.titleGet().begin(),m_sink.get());
		putc('\n',m_sink.get());
		}

	auto desc=record.propertyGet(String("Description"));
	if(desc!=nullptr)
		{valueWrite(desc->begin(),m_sink.get());}
	fputs("\n\n",m_sink.get());

	record.propertiesEnum([this](const String& name,const String& value)
		{
		if(name!="Description")
			{
			putc('~',m_sink.get());
			keyWrite(name.begin(),m_sink.get());
			auto key_end=multipar(value.begin())?"\n":": ";
			fputs(key_end,m_sink.get());
			valueWrite(value.begin(),m_sink.get());
			if(*key_end=='\n')
				{putc('\n',m_sink.get());}
			putc('\n',m_sink.get());
			}
		return true;
		});
	putc('\n',m_sink.get());
	}
