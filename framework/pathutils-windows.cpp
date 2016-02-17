#ifdef __WAND__
target[name[pathutils.o] type[object] platform[;Windows] dependency[shlwapi;external]]
#endif

#include "pathutils.h"
#include <windows.h>

#include <shlwapi.h> //	Needed for PathRelativePathTo wtf

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

ArrayDynamicShort<char> makeRelativeTo(const char* path, const char* reference)
	{
	char buffer[4*MAX_PATH];
	if(PathRelativePathToA(buffer,reference,FILE_ATTRIBUTE_DIRECTORY,path,0))
		{return buffer;}
	return path;
	}
