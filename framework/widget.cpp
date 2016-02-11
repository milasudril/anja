#ifdef __WAND__
target[name[widget.o] type[object]]
#endif

#include "widget.h"
#include "messagedisplay.h"

uint32_t Widget::notifyUser(const char* message,const char* title
	,uint32_t message_type)
	{
	return messageDisplay(*this,message,title,message_type);
	}
