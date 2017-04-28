//@	{
//@	 "targets":[{"name":"pathutils.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"pathutils.o","rel":"implementation"}]
//@	}

#ifndef ANJA_PATHUTILS_HPP
#define ANJA_PATHUTILS_HPP

#include "arraydynamicshort.hpp"

namespace Anja
	{
	ArrayDynamicShort<char> realpath(const ArrayDynamicShort<char>& path);
	ArrayDynamicShort<char> parentDirectory(const ArrayDynamicShort<char>& path);
	bool absoluteIs(const ArrayDynamicShort<char>& path);
	ArrayDynamicShort<char> makeRelativeTo(const char* path, const char* reference);
	}

#endif
