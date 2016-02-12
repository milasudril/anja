#ifdef __WAND__
target[name[pathutils.o] type[object] platform[;Windows]]
#endif

#include "pathutils.h"
#include <windows.h>

ArrayDynamicShort<char> realpath(const ArrayDynamicShort<char>& path)
	{
	char buffer[4*MAX_PATH];
	GetFullPathNameA(path,4*MAX_PATH,buffer,NULL);
	return ArrayDynamicShort<char>(buffer);
	}

ArrayDynamicShort<char> parentDirectory(const ArrayDynamicShort<char>& path)
	{
	auto ret=path;
	auto end=path.end();
	auto begin=path.begin();
	while(end!=begin)
		{
		--end;
		ret.truncate();
		if(*end=='/' || *end=='\\')
			{break;}
		}
	ret.append("\\");
	return ret;
	}

bool absoluteIs(const ArrayDynamicShort<char>& path)
	{
	return !PathIsRelativeA(path.begin());
	}
