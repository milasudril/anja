#ifdef __WAND__
target[name[widget.h] type[include]]
#endif

#ifndef WIDGET_H
#define WIDGET_H

class GuiHandle;

class Widget
	{
	public:
		virtual void destroy()=0;
		virtual const GuiHandle& handleNativeGet() const=0;

	protected:
		virtual ~Widget()=default;
	};

#endif
