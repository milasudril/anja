//@	{
//@	"targets":
//@		[{"name":"anja","type":"application","include_targets":["projectinfo.hpp"]}]
//@	}

#include "ui/application.hpp"
#include "alice/alice.hpp"
#include <cstdio>
#include <time.h>

namespace Alice
	{
	template<>
	struct MakeType<Stringkey("filename")>:public MakeType<Stringkey("string")>
		{
		static constexpr const char* descriptionShortGet() noexcept
			{return "A valid filename, encoded in UTF-8";}
		};

	template<>
	struct MakeType<Stringkey("filename in")>:public MakeType<Stringkey("filename")>
		{
		static constexpr const char* descriptionShortGet() noexcept
			{return "A name of an existing file";}

		static constexpr const char* descriptionLongGet() noexcept
			{
			return "If there is no file with the given name, an error occurs.";
			}
		};

	template<>
	struct MakeType<Stringkey("filename out")>:public MakeType<Stringkey("filename")>
		{
		static constexpr const char* descriptionShortGet() noexcept
			{return "A valid filename, encoded in UTF-8";}

		static constexpr const char* descriptionLongGet() noexcept
			{
			return "If a file with the same name already exists, it will be "					"overwritten.";
			}
		};

	enum class Theme:int{DARK,LIGHT};

	template<>
	struct MakeType<Stringkey("theme")>
		{
		typedef Theme Type;
		static constexpr const char* descriptionShortGet() noexcept
			{return "dark | light";}

		static constexpr const char* descriptionLongGet() noexcept
			{return "";}
		};

	template<class ErrorHandler>
	struct MakeValue<Theme,ErrorHandler>
		{
		static Theme make_value(const std::string& str);
		};

	template<class ErrorHandler>
	Theme MakeValue<Theme,ErrorHandler>::make_value(const std::string& str)
		{
		if(str=="dark")
			{return Theme::DARK;}
		if(str=="light")
			{return Theme::LIGHT;}
		throw Anja::Error("`",str.c_str(),"` is not a valid theme. Possible themes are `dark` and `light`.");
		}


	enum class WindowMode:int{FULLSCREEN,WINDOWED};

	template<>
	struct MakeType<Stringkey("window mode")>
		{
		typedef WindowMode Type;
		static constexpr const char* descriptionShortGet() noexcept
			{return "fullscreen | windowed";}

		static constexpr const char* descriptionLongGet() noexcept
			{return "";}
		};

	template<class ErrorHandler>
	struct MakeValue<WindowMode,ErrorHandler>
		{
		static WindowMode make_value(const std::string& str);
		};

	template<class ErrorHandler>
	WindowMode MakeValue<WindowMode,ErrorHandler>::make_value(const std::string& str)
		{
		if(str=="fullsscreen")
			{return WindowMode::FULLSCREEN;}
		if(str=="windowed")
			{return WindowMode::WINDOWED;}
		throw Anja::Error("`",str.c_str(),"` is not a valid window mode. Possible modes are `fullscreen` and `windowed`.");
		}
	}

ALICE_OPTION_DESCRIPTOR(OptionDescriptor
	,{"Program information","about","prints an about message to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Program information","help","prints all availible command line options to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Program information","version","prints version information to *filename out*. Without argument, the data is written to standard outout.","filename out",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Appearance","theme","selects the UI theme. The default theme is `dark`.","theme",Alice::Option::Multiplicity::ONE}
	,{"Appearance","window-mode","selects the window mode. The default mode is windowed`.","window mode",Alice::Option::Multiplicity::ONE}
	,{"Session loading/control","session","loads a saved session from *filename in*. Without argument, the session is read from standard input. Notice that this option is ignored if `script` is given.","filename in",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"Session loading/control","script","reads and executes commands from *filename in*. Without argument, the session is read from standard input. The command stream works independently of the UI. Notice that this option overrides the `session` option.","filename in",Alice::Option::Multiplicity::ZERO_OR_ONE});

int main(int argc, char **argv)
	{
	try
		{
		Alice::CommandLine<OptionDescriptor> cmdline(argc,argv);

		Anja::Application anja;
		anja.dark(1);

		anja.layoutInspect([](int k,const Anja::Rectangle& rect)
			{
			printf("# %d %.15g %.15g %.15g %.15g\n",k
				,rect.min.x(),rect.min.y()
				,rect.max.x(),rect.max.y());
			});
		anja.run();
		}
	catch(const Anja::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}
	return 0;
	}
