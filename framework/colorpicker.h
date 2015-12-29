#ifdef __WAND__
target[name[colorpicker.h] type[include]]
dependency[colorpicker.o]
#endif

#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "widget.h"
#include <cstddef>

class ColorRGBA;
class GuiContainer;

class ColorPicker:public Widget
	{
	public:
		static ColorPicker* create(GuiContainer& parent,ColorRGBA& color_current
			,const ColorRGBA* presets,size_t N_presets);

		virtual void update()=0;

	protected:
		virtual ~ColorPicker()=default;
	};

#endif
