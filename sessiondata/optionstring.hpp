//@	{
//@	 "targets":[{"name":"optionstring.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"optionstring.o","rel":"implementation"}]
//@	}

#ifndef ANJA_OPTIONSTRING_HPP
#define ANJA_OPTIONSTRING_HPP

#include "../common/arraydynamicshort.hpp"

namespace Anja
	{
	ArrayDynamicShort<char> stringFromOptions(unsigned int option_bits
		,const char* const* option_names);

	unsigned int optionsFromString(const char* string
		,const char* const* option_names);

	unsigned int optionFromString(const char* string,const char* const* option_names);
	}

#endif
