//@ [
//@		[
//@		"and(gnu,linux)"
//@			,{"targets":[{"name":"pathutils.o","type":"object"}]}
//@		]
//@	]

#include "pathutils.hpp"
#include "error.hpp"
#include "syserror.hpp"
#include <cstdlib>
#include <memory>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

using namespace Anja;

String Anja::realpath(const char* path)
	{
	auto fd=open(path,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR);
	if(fd==-1)
		{
		if(errno!=EEXIST)
			{throw Error("Resolving ",path," failed. ",SysError(errno));}
		std::unique_ptr<char,decltype(&free)> result{::realpath(path,NULL),free};
		if(result.get()==nullptr)
			{throw Error("Resolving ",path," failed. ",SysError(errno));}
		return String(result.get());
		}

	std::unique_ptr<char,decltype(&free)> result{::realpath(path,NULL),free};
	close(fd);
	unlink(path);
	if(result.get()==nullptr)
		{throw Error("Resolving ",path," failed. ",SysError(errno));}
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
	if(*reference=='\0')
		{return String(path);}

//	Assume that path and reference are absolute paths
	String ret;
	String path_tok;
	String ref_tok;

//	Make sure the reference path ends with '/'
	String ref_temp(reference);
	if(*(ref_temp.end()-1)!='/')
		{ref_temp.append("/");}
	reference=ref_temp.begin();

	auto path_old=path;
//	Count the number of generations the the common stem
	while(*reference!='\0' && *path!='\0')
		{
		path_old=path;
		path=pathTokenGet(path,path_tok);
		reference=pathTokenGet(reference,ref_tok);

		if(strcmp(path_tok.begin(),ref_tok.begin()))
			{break;}
		}

	if(strcmp(path_tok.begin(),ref_tok.begin()))
		{
	//	There last tokens of `reference` differs from `path. Consume the
	//	remaining parts of `reference` while adding "up" operators. Notice that
	//	we have eaten one element too much. Therefore, add one extra "up"
	//	operator.
		ret.append("../");
		while(*reference!='\0')
			{
			reference=pathTokenGet(reference,ref_tok);
			ret.append("../");
			}
		return ret.append(path_old);
		}

	return String(path);
	}

bool Anja::fileIs(const char* path)
	{
	struct stat st;
	if(stat(path,&st)==-1)
		{return 0;}
	return S_ISREG(st.st_mode);
	}
