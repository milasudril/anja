#ifdef __WAND__
target[name[exceptionswallow.h] type[include]]
#endif

#ifndef EXCEPTIONSWALLOW_H
#define EXCEPTIONSWALLOW_H

#include <cstdio>

#define EXCEPTION_SWALLOW(_statements,_context) do\
	{ \
	try { _statements }\
	catch(...)\
		{printf("An exception occured\n");} \
	} \
	while(0)

#endif
