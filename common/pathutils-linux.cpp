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
#include <sys/stat.h>

using namespace Anja;

String Anja::realpath(const String& path)
	{
	std::unique_ptr<char,decltype(&free)> result
		{::realpath(path.begin(),NULL),free};
	return String(result.get());
	}

String Anja::parentDirectory(const String& path)
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

bool Anja::absoluteIs(const char* path)
	{return *path=='/';}

static const char* pathTokenGet(const char* path,String& token)
	{
	token.clear();
	while(*path!='/' && *path!='\0')
		{
		token.append(*path);
		++path;
		}
	if(*path!='\0')
		{++path;}
	return path;
	}

String Anja::makeRelativeTo(const char* path, const char* reference)
	{
//	Assume that path and reference are absolute paths
	String ret;
	String path_tok;
	String ref_tok;

//	Make shure the reference path ends with '/'
	String ref_temp(reference);
	ref_temp.truncate();
	if(*(ref_temp.end()-1)!='/')
		{ref_temp.append("/");}
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
			ret.append("../").append(path_tok);
			return ret;
			}
		else
	//	Ohterwise, check if the two last tokens are equal. If so, just append
	//	the remaining part of path
		if(strcmp(path_tok.begin(),ref_tok.begin())==0)
			{
			ret.append(path);
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
		ret.append("../");
		--gen_count_tot;
		}
	if(path_val_last!='\0' || ref_val_last=='\0')
		{ret.append(path_old);}
	return ret;
	}

bool Anja::fileIs(const char* path)
	{
	struct stat st;
	if(stat(path,&st)==-1)
		{return 0;}
	return S_ISREG(st.st_mode);
	}
