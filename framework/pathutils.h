#ifdef __WAND__
target[name[pathutils.h] type[include]]
dependency[pathutils.o]
#endif

#ifndef PATHUTILS_H
#define PATHUTILS_H

#include "arraydynamicshort.h"

ArrayDynamicShort<char> realpath(const ArrayDynamicShort<char>& path);
ArrayDynamicShort<char> parentDirectory(const ArrayDynamicShort<char>& path);
bool absoluteIs(const ArrayDynamicShort<char>& path);

#endif
