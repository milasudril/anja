#ifdef __WAND__
target[name[exceptionswallow.h] type[include]]
#endif

#ifndef EXCEPTIONSWALLOW_H
#define EXCEPTIONSWALLOW_H

#include "messagedisplay.h"

#define EXCEPTION_SWALLOW(_statements,_context) do\
	{ \
	try { _statements }\
	catch(...)\
		{\
		messageDisplay(*_context,"Unkonwn exception","Anja",MESSAGE_TYPE_ERROR);\
		} \
	} \
	while(0)

#endif
