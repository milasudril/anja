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
		class Tag
			{};

		class EventHandler
			{
			public:
				virtual void onConfirmed(Tag dummy)
					{}

				virtual void onCanceled(Tag dummy)
					{}
			};

		static ColorPicker* create(GuiContainer& parent,ColorRGBA& color_current
			,ColorRGBA* presets,size_t N_presets)
			{
			return create(parent,color_current,presets,N_presets,s_default_handler);
			}

		static ColorPicker* create(GuiContainer& parent,ColorRGBA& color_current
			,ColorRGBA* presets,size_t N_presets,EventHandler& event_handler);

		virtual void update()=0;

	protected:
		virtual ~ColorPicker()=default;

	private:
		static EventHandler s_default_handler;
	};

#endif
