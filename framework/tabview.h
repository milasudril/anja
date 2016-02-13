#ifdef __WAND__
target[name[tabview.h] type[include]]
dependency[tabview.o]
#endif

#ifndef TABVIEW_H
#define TABVIEW_H

#include "guicontainer.h"
#include <cstdint>

class TabView:public GuiContainer
	{
	public:
		static TabView* create(GuiContainer& parent);

		virtual void tabTitleSet(unsigned int tab,const char* title)=0;

		virtual void tabActiveSet(unsigned int tab)=0;
	};

#endif
