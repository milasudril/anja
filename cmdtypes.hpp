//@	{"targets":[{"name":"cmdtypes.hpp","type":"include"}]}

#ifndef ANJA_CMDTYPES_HPP
#define ANJA_CMDTYPES_HPP

#include "alice/alice.hpp"

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

#endif
