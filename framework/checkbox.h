#ifdef __WAND__
target[name[checkbox.h] type[include]]
dependency[checkbox.o]
#endif

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "widget.h"

class GuiContainer;

class Checkbox:public Widget
	{
	public:
		static Checkbox* create(GuiContainer& parent,const char* title,unsigned int command_id);
		virtual bool stateGet() const=0;
		virtual void stateSet(bool state_new)=0;
	};

#endif
