#ifdef __WAND__
target[name[label.h] type[include]]
dependency[label.o]
#endif

#ifndef LABEL_H
#define LABEL_H

#include "widget.h"

class GuiContainer;

class Label:public Widget
	{
	public:
		static Label* create(GuiContainer& parent,const char* title);
		virtual void titleSet(const char* title)=0;
	};

#endif
