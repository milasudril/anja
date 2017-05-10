//@	{
//@	 "targets":[{"name":"pathutils.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"pathutils.o","rel":"implementation"}]
//@	}

#ifndef ANJA_PATHUTILS_HPP
#define ANJA_PATHUTILS_HPP

#include "string.hpp"

namespace Anja
	{
	String realpath(const String& path);
	String parentDirectory(const String& path);
	bool absoluteIs(const char* path);
	inline bool absoluteIs(const String& path)
		{return absoluteIs(path.begin());}
	String makeRelativeTo(const char* path, const char* reference);
	}

#endif
