#ifdef __WAND__
target[name[boxvertical.h] type[include]]
dependency[boxvertical.o]
#endif

#ifndef BOXVERTICAL_H
#define BOXVERTICAL_H

#include "guicontainer.h"
#include <cstdint>

class BoxVertical:public GuiContainer
	{
	public:
		typedef GuiContainer::EventHandler<BoxVertical> EventHandler;

		static BoxVertical* create(GuiContainer& parent)
			{return create(parent,nullptr);}

		static BoxVertical* create(GuiContainer& parent,EventHandler* event_handler);

		static constexpr uint32_t INSERTMODE_END=1;
		static constexpr uint32_t INSERTMODE_EXPAND=2;
		static constexpr uint32_t INSERTMODE_FILL=4;
		static constexpr uint32_t INSERTMODE_LEFT=8;

		virtual void insertModeSet(uint32_t mode)=0;
	};

#endif
