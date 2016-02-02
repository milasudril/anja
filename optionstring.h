#ifdef __WAND__
target[name[optionstring.h] type[include]]
dependency[optionstring.o]
#endif

#ifndef OPTIONSTRING_H
#define OPTIONSTRING_H

#include "framework/arraydynamicshort.h"

ArrayDynamicShort<char> stringFromOptions(unsigned int option_bits
	,const char* const* option_names);

unsigned int optionsFromString(const char* string
	,const char* const* option_names);

unsigned int optionFromString(const char* string,const char* const* option_names);

#endif
