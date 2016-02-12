#ifdef __WAND__
target[name[messagedisplay.o] type[object] platform[;Windows]]
#endif

#include <windows.h>
#include "messagedisplay.h"
#include "widget.h"

uint32_t messageDisplay(const Widget& owner,const char* message
	,const char* context_title
	,uint32_t message_type)
	{
	return MessageBoxA(owner.handleNativeGet(),message,context_title,message_type);
	}
