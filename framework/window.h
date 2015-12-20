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
		class EventHandler:public GuiContainer::EventHandler
			{
			public:
				virtual bool onClose()
					{return true;}
			};

		static Window* create(EventLoop& event_loop)
			{return create(event_loop,s_default_handler,nullptr);}

		static Window* create(EventLoop& event_loop,EventHandler& handler)
			{return create(event_loop,handler,nullptr);}

		static Window* create(Window& owner)
			{return create(owner.r_event_loop,s_default_handler,&owner);}

		static Window* create(Window& owner,EventHandler& handler)
			{return create(owner.r_event_loop,handler,&owner);}

		virtual void eventHandlerSet(EventHandler& handler)=0;
		virtual void titleSet(const char* title_new)=0;


	protected:
		Window(EventLoop& event_loop):r_event_loop(event_loop)
			{r_event_loop.windowRegister();}

		~Window()
			{r_event_loop.windowUnregister();}

	private:
		static Window* create(EventLoop& event_loop,EventHandler& handler,Window* owner);

		EventLoop& r_event_loop;
		static EventHandler s_default_handler;
	};

#endif
