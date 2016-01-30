#ifdef __WAND__
target[name[colorview.h] type[include]]
dependency[colorview.o]
#endif

#ifndef COLORVIEW_H
#define COLORVIEW_H

#include "widget.h"

class ColorRGBA;
class GuiContainer;

class ColorView:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onActionPerform(ColorView& source)
					{}
			};

		static ColorView* create(GuiContainer& parent)
			{return create(parent,s_default_handler,0);}

		static ColorView* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id);

		virtual unsigned int idGet() const=0;
		virtual const ColorRGBA& colorGet() const=0;
		virtual void colorSet(const ColorRGBA& color)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
