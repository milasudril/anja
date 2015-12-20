#ifdef __WAND__
target[name[boxhorizontal.h] type[include]]
dependency[boxhorizontal.o]
#endif

#ifndef BOXHORIZONTAL_H
#define BOXHORIZONTAL_H

#include "guicontainer.h"
#include <cstdint>

class BoxHorizontal:public GuiContainer
	{
	public:
		static BoxHorizontal* create(GuiContainer& parent)
			{return create(parent,nullptr);}

		static BoxHorizontal* create(GuiContainer& parent,EventHandler* event_handler);

		static constexpr uint32_t INSERTMODE_END=1;
		static constexpr uint32_t INSERTMODE_EXPAND=2;
		static constexpr uint32_t INSERTMODE_FILL=4;

		virtual void insertModeSet(uint32_t mode)=0;
	};

#endif
