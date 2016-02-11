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
		typedef GuiContainer::EventHandler<BoxHorizontal> EventHandler;

		static BoxHorizontal* create(GuiContainer& parent)
			{return create(parent,s_default_handler);}

		static BoxHorizontal* create(GuiContainer& parent,EventHandler& handler);

		static constexpr uint32_t INSERTMODE_END=1;
		static constexpr uint32_t INSERTMODE_EXPAND=2;
		static constexpr uint32_t INSERTMODE_FILL=4;
		static constexpr uint32_t INSERTMODE_TOP=8;

		virtual void insertModeSet(uint32_t mode)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
