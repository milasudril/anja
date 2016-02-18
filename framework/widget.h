#ifdef __WAND__
target[name[widget.h] type[include]]
dependency[widget.o]
#endif

#ifndef WIDGET_H
#define WIDGET_H

class GuiHandle;

#include "messageconstants.h"

class Widget
	{
	public:
		virtual void destroy()=0;

		static void widgetDestroy(Widget* widget)
			{widget->destroy();}

		virtual const GuiHandle& handleNativeGet() const=0;

		virtual uint32_t notifyUser(const char* message,const char* title
			,uint32_t message_type);

		virtual unsigned int heightMinGet()
			{return 24;}

		virtual unsigned int widthMinGet()
			{return 96;}

		virtual unsigned int widthForHeightGet(unsigned int height)
			{return 96;}

		virtual unsigned int heightForWidthGet(unsigned int width)
			{return 24;}

	protected:
		virtual ~Widget()=default;
	};

#endif
