//@ [
//@		[
//@		"and(gnu,linux)"
//@			,{"targets":[{"name":"pathutils.o","type":"object"}]}
//@		]
//@	]

#include "pathutils.hpp"
#include <cstdlib>
#include <memory>
#include <cstring>

using namespace Anja;

ArrayDynamicShort<char> Anja::realpath(const ArrayDynamicShort<char>& path)
	{
	std::unique_ptr<char,decltype(&free)> result
		{::realpath(path.begin(),NULL),free};
	return ArrayDynamicShort<char>(result.get());
	}

ArrayDynamicShort<char> Anja::parentDirectory(const ArrayDynamicShort<char>& path)
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
	ret.append("/",2);
	return ret;
	}

bool Anja::absoluteIs(const ArrayDynamicShort<char>& path)
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

ArrayDynamicShort<char> Anja::makeRelativeTo(const char* path, const char* reference)
	{
//	Assume that path and reference are absolute paths
	ArrayDynamicShort<char> ret;
	ArrayDynamicShort<char> path_tok;
	ArrayDynamicShort<char> ref_tok;

//	Make shure the reference path ends with '/'
	ArrayDynamicShort<char> ref_temp(reference);
	ref_temp.truncate();
	if(*(ref_temp.end()-1)!='/')
		{ref_temp.append("/",2);}
	reference=ref_temp.begin();

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

	auto ref_val_last=*reference;
	auto path_val_last=*path;

//	We have read through reference. This means that all but possibly the last
//	components of referenec are equalto the corresponding part in path.
	if(ref_val_last=='\0')
		{
	//	If we have reached end of path
		if(path_val_last=='\0')
			{
		//	Since we have processed both paths, the only difference must be in the
		//	last component. Just go up and down
			ret.append("../",3).append(path_tok);
			return ret;
			}
		else
	//	Ohterwise, check if the two last tokens are equal. If so, just append
	//	the remaining part of path
		if(strcmp(path_tok.begin(),ref_tok.begin())==0)
			{
			ret.append(path,strlen(path)+1);
			return ret;
			}
		}

	auto gen_count_tot=gen_count_target;

//	Add the last part to gen_count_tot, but only if there are remaining
//	components of path. This is because when there are no parts left in path,
//	the parts that differs remains in reference
	if(path_val_last!='\0')
		{++gen_count_tot;}

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
		ret.append("../",3);
		--gen_count_tot;
		}
	if(path_val_last!='\0' || ref_val_last=='\0')
		{ret.append(path_old,strlen(path_old));}
	ret.append('\0');
	return ret;
	}
