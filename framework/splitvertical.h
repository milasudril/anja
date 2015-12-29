#ifdef __WAND__
target[name[splitvertical.h] type[include]]
dependency[splitvertical.o]
#endif

#ifndef SPLITVERTICAL_H
#define SPLITVERTICAL_H

#include "guicontainer.h"
#include <cstdint>

class SplitVertical:public GuiContainer
	{
	public:
		typedef GuiContainer::EventHandler<SplitVertical> EventHandler;

		static SplitVertical* create(GuiContainer& parent)
			{return create(parent,nullptr);}

		static SplitVertical* create(GuiContainer& parent,EventHandler* event_handler);
	};

#endif
