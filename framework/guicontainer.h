#ifdef __WAND__
target[name[guicontainer.h] type[include]]
#endif

#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include "widget.h"

class GuiContainer:public Widget
	{
	public:
		virtual void destroy()=0;
		virtual void componentAdd(const Widget& component)=0;
		virtual void componentRemove(const Widget& component)=0;

	protected:
		virtual ~GuiContainer()=default;
	};

#endif
