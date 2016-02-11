#ifdef __WAND__
target[name[window.h] type[include]]
dependency[window.o]
#endif

#ifndef WINDOW_H
#define WINDOW_H

#include "eventloop.h"
#include "guicontainer.h"

class GuiHandle;

class Window:public GuiContainer
	{
	public:
		class EventHandler:public GuiContainer::EventHandler<Window>
			{
			public:
				virtual void onClose(Window& source)
					{source.destroy();}
			};

		static Window* create(EventLoop& event_loop)
			{return create(event_loop,s_default_handler);}

		static Window* create(EventLoop& event_loop,EventHandler& handler);

		static Window* create(Widget& owner)
			{return create(owner,s_default_handler,nullptr);}

		static Window* create(Widget& owner,void** lifetime_cookie)
			{return create(owner,s_default_handler,lifetime_cookie);}

		static Window* create(Widget& owner,EventHandler& handler)
			{return create(owner,s_default_handler,nullptr);}

		static Window* create(Widget& owner,EventHandler& handler
			,void** lifetime_cookie);

		virtual void eventHandlerSet(EventHandler& handler)=0;
		virtual void titleSet(const char* title_new)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
