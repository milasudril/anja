#ifdef __WAND__
target[name[scrollwindow.h] type[include]]
dependency[scrollwindow.o]
#endif

#ifndef SCROLLWINDOW_H
#define SCROLLWINDOW_H

#include "eventloop.h"
#include "guicontainer.h"

class ScrollWindow:public GuiContainer
	{
	public:
		class EventHandler:public GuiContainer::EventHandler<ScrollWindow>
			{
			};

		static constexpr unsigned int MODE_VERTICAL=1;
		static constexpr unsigned int MODE_HORIZONTAL=2;

		static ScrollWindow* create(GuiContainer& parent,unsigned int mode)
			{return create(parent,s_default_handler,mode);}

		static ScrollWindow* create(GuiContainer& parent,EventHandler& handler
			,unsigned int mode);

		virtual void eventHandlerSet(EventHandler& handler)=0;
		virtual void heightMinSet(unsigned int h)=0;
		virtual void widthMinSet(unsigned int w)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
