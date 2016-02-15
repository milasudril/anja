#ifdef __WAND__
target[name[pathutils.o] type[object] platform[;GNU/Linux]]
#endif

#include "pathutils.h"
#include <cstdlib>
#include <memory>
#include <cstring>

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

static const char* pathTokenGet(const char* path,ArrayDynamicShort<char>& token)
	{
	token.clear();
	while(*path!='/' && *path!='\0')
		{
		token.append(*path);
		++path;
		}
	if(*path!='\0')
		{++path;}
	token.append('\0');
	return path;
	}

ArrayDynamicShort<char> makeRelativeTo(const char* path, const char* reference)
	{
//	Assume that path and reference are absolute paths
	ArrayDynamicShort<char> ret;
	ArrayDynamicShort<char> path_tok;
	ArrayDynamicShort<char> ref_tok;

//	Make shure the reference path ends with '/'
	ArrayDynamicShort<char> ref_temp(reference);
	ref_temp.truncate();
	if(*(ref_temp.end()-1)!='/')
		{ref_temp.append("/");}
	reference=ref_temp.begin();


	auto ref_begin=reference;
	unsigned int gen_count_target=0;
	auto path_old=path;
//	Count the number of generations the the common stem
	while(*reference!='\0' && *path!='\0')
		{
		path_old=path;
		path=pathTokenGet(path,path_tok);
		reference=pathTokenGet(reference,ref_tok);

		if(strcmp(path_tok.begin(),ref_tok.begin())==0)
			{
		//	Increment the number of identical parts
			++gen_count_target;
			}
		else
			{break;}
		}
// Subtract leading identical token from  target
	--gen_count_target;

	auto ref_val_last=*reference;
	auto path_val_last=*path;

	auto gen_count_tot=gen_count_target;

//	Add the last part to gen_count_tot, but only if there are remaining
//	components of path. Also some empirical rules apply...
	if((path_val_last!='\0'
		||  (ref_val_last=='\0' && path_val_last=='\0'))
		&& !(ref_val_last=='\0' && path_val_last!='\0') )
		{++gen_count_tot;}

	if(gen_count_tot==gen_count_target && path_val_last!='\0')
		{return path;}

//	Count the number of remaining parts
	while(*reference!='\0')
		{
		if(*reference=='/')
			{++gen_count_tot;}
		++reference;
		}

//	Go up from the reference path
	while(gen_count_tot!=gen_count_target)
		{
		ret.append("../").truncate();
		--gen_count_tot;
		}
	if(path_val_last!='\0' || ref_val_last=='\0')
		{ret.append(path_old);}
	ret.append('\0');
	return ret;
	}
