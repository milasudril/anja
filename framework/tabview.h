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
		typedef GuiContainer::EventHandler<TabView> EventHandler;

		static TabView* create(GuiContainer& parent)
			{return create(parent,nullptr);}

		static TabView* create(GuiContainer& parent,EventHandler* event_handler);

		virtual void tabTitleSet(unsigned int tab,const char* title)=0;
	};

#endif
