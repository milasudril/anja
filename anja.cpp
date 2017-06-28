//@	{
//@	"targets":
//@		[{"name":"anja","type":"application","include_targets":["projectinfo.hpp"]}]
//@	}

#include "ui/application.hpp"
#include "cmdtypes.hpp"
#include <cstdio>

ALICE_OPTION_DESCRIPTOR(OptionDescriptor
	,{"Program information","about","prints an about message to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Program information","help","prints all availible command line options to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Program information","version","prints version information to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Appearance","theme","selects the UI theme. The default theme is `dark`. Notice that theme modes does not work on all GTK+3 themes. In this case, this option has no effect.","theme",Alice::Option::Multiplicity::ONE}
	,{"Appearance","window-mode","selects the window mode. The default mode is windowed`.","window mode",Alice::Option::Multiplicity::ONE}
	,{"Session loading/control","session","loads a saved session from *filename in*. Notice that this option is ignored if `script` is given.","filename in",Alice::Option::Multiplicity::ONE}
	,{"Session loading/control","script","reads and executes commands from *filename in*. Without argument, the session is read from standard input. The command stream works independently of the UI. Notice that this option overrides the `session` option.","filename in",Alice::Option::Multiplicity::ZERO_OR_ONE});

namespace
	{
	class FileIn
		{
		public:
			FileIn(const FileIn&)=delete;
			FileIn& operator=(const FileIn&)=delete;

		//I am lazy, and will never need these anyway
			FileIn(FileIn&&)=delete;
			FileIn& operator=(FileIn&&)=delete;

			FileIn(const char* name)
				{
				m_handle=(name==nullptr? stdin : fopen(name,"rb"));
				if(m_handle==NULL)
					{throw Anja::Error("Failed to open the file ",name);}
				}

			~FileIn()
				{
				if(m_handle!=stdin)
					{fclose(m_handle);}
				}

			FILE* get() noexcept
				{return m_handle;}

		private:
			FILE* m_handle;
		};

	class FileOut
		{
		public:
			FileOut(const FileIn&)=delete;
			FileOut& operator=(const FileIn&)=delete;

		//I am lazy, and will never need these anyway...
			FileOut(FileOut&&)=delete;
			FileOut& operator=(FileOut&&)=delete;

			FileOut(const char* name)
				{
				m_handle=(name==nullptr? stdout : fopen(name,"wb"));
				if(m_handle==NULL)
					{throw Anja::Error("Failed to open the file ",name);}
				}

			~FileOut()
				{
				if(m_handle!=stdout)
					{fclose(m_handle);}
				}

			FILE* get() noexcept
				{return m_handle;}

		private:
			FILE* m_handle;
		};

	class CmdReader
		{
		public:
			CmdReader(const char* filename,Anja::Application& dest):
				 m_src(filename),r_dest(dest)
				,m_read_thread(*this,std::integral_constant<int,0>{})
				{}

			template<int id>
			void run()
				{
				auto fptr=m_src.get();
				std::string buffer;
				std::vector<std::string> cmd;
				enum class State:int{NORMAL,ESCAPE};
				auto state_current=State::NORMAL;
				while(1)
					{
					auto ch_in=getc(fptr);
					switch(state_current)
						{
						case State::NORMAL:
							switch(ch_in)
								{
								case EOF:
									cmd.push_back(buffer);
									invoke(cmd);
									return;
								case '\n':
									cmd.push_back(buffer);
									buffer.clear();
									invoke(cmd);
									cmd.clear();
									break;
								case '\\':
									state_current=State::ESCAPE;
									break;
								case ',':
									cmd.push_back(buffer);
									buffer.clear();
									break;
								default:
									buffer+=ch_in;
									break;
								}
							break;
						case State::ESCAPE:
							if(ch_in==EOF)
								{return;}
							buffer+=ch_in;
							state_current=State::NORMAL;
							break;
						}
					}
				}

		private:
			void invoke(const std::vector<std::string>& cmd)
				{
				try
					{

					}
				catch(const Anja::Error& e)
					{fprintf(stderr,"Error: %s",e.message());}
				}

			FileIn m_src;
			Anja::Application& r_dest;
			Anja::Thread m_read_thread;
		};
	}

static CmdReader* cmdreader_create(const std::vector<std::string>& filename
	,Anja::Application& app)
	{
	if(filename.size())
		{return new CmdReader(filename[0].c_str(),app);}
	return new CmdReader(nullptr,app);
	}

int main(int argc, char **argv)
	{
	try
		{
		Alice::CommandLine<OptionDescriptor> cmdline(argc,argv);
		if(cmdline.get<Alice::Stringkey("help")>())
			{
			auto& val=cmdline.get<Alice::Stringkey("help")>().valueGet();
			FileOut output(val.size()?val[0].c_str():nullptr);
			cmdline.help(1,output.get());
			return 0;
			}

		if(cmdline.get<Alice::Stringkey("version")>())
			{
			auto& val=cmdline.get<Alice::Stringkey("version")>().valueGet();
			FileOut output(val.size()?val[0].c_str():nullptr);
			fprintf(output.get(),"%s version %s\n"
				"This anja was compiled on %s"
				,Anja::ProjectInfo::name()
				,Anja::ProjectInfo::revision()
				,Anja::ProjectInfo::compilationDate());
			return 0;
			}

		if(cmdline.get<Alice::Stringkey("about")>())
			{
			auto& val=cmdline.get<Alice::Stringkey("about")>().valueGet();
			FileOut output(val.size()?val[0].c_str():nullptr);
			fprintf(output.get(),"%s version %s\n"
				"This anja was compiled on %s\n\n"
				"%s\n\n"
				"%s\n"
				,Anja::ProjectInfo::name()
				,Anja::ProjectInfo::revision()
				,Anja::ProjectInfo::compilationDate()
				,Anja::ProjectInfo::copyright()
				,Anja::ProjectInfo::legalBrief());
			return 0;
			}

		Anja::Application anja;
		if(cmdline.get<Alice::Stringkey("theme")>())
			{
			auto& val=cmdline.get<Alice::Stringkey("theme")>();
			anja.dark(val.valueGet()==Alice::Theme::DARK);
			}
		else
			{anja.dark(1);}

		if(cmdline.get<Alice::Stringkey("window-mode")>())
			{
			auto& val=cmdline.get<Alice::Stringkey("window-mode")>();
			anja.fullscreen(val.valueGet()==Alice::WindowMode::FULLSCREEN);
			}
		else
			{anja.fullscreen(0);}

		if(cmdline.get<Alice::Stringkey("session")>() && !cmdline.get<Alice::Stringkey("script")>())
			{anja.sessionLoad(cmdline.get<Alice::Stringkey("session")>().valueGet().c_str());}

		std::unique_ptr<CmdReader> cmdreader(cmdline.get<Alice::Stringkey("script")>()?
			 cmdreader_create(cmdline.get<Alice::Stringkey("script")>().valueGet(),anja)
			:nullptr);

		anja.run();
		}
	catch(const Anja::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}
	catch(const Alice::ErrorMessage& msg)
		{
		fprintf(stderr,"Error: %s\n",msg.data);
		return -1;
		}
	return 0;
	}
