#ifdef __WAND__
target[name[pathutils.o] type[object] platform[;GNU/Linux]]
#endif

#include "pathutils.h"
#include <cstdlib>
#include <memory>

ArrayDynamicShort<char> realpath(const ArrayDynamicShort<char>& path)
	{
	std::unique_ptr<char,decltype(&free)> result
		{realpath(path.begin(),NULL),free};
	return ArrayDynamicShort<char>(result.get());
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
		if(*end=='/')
			{break;}
		}
	ret.append("/");
	return ret;
	}

bool absoluteIs(const ArrayDynamicShort<char>& path)
	{
	return *(path.begin())=='/';
	}
