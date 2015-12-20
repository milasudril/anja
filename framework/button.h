#ifdef __WAND__
target[name[button.h] type[include]]
dependency[button.o]
#endif

#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class GuiContainer;

class Button:public Widget
	{
	public:
		static Button* create(GuiContainer& parent,const char* title,unsigned int command_id);
	};

#endif
