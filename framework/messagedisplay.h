#ifdef __WAND__
target[name[messagedisplay.h] type[include]]
dependency[messagedisplay.o]
#endif

#ifndef MESSAGEDISPLAY_H
#define MESSAGEDISPLAY_H

#include "messageconstants.h"

class Widget;

uint32_t messageDisplay(const Widget& owner,const char* message
	,const char* context_title
	,uint32_t message_type);

#endif
