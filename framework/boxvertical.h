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
		static BoxVertical* create(GuiContainer& parent);

		static constexpr uint32_t INSERTMODE_END=1;
		static constexpr uint32_t INSERTMODE_EXPAND=2;
		static constexpr uint32_t INSERTMODE_FILL=4;

		virtual void insertModeSet(uint32_t mode)=0;
	};

#endif
