#ifdef __WAND__
target[name[button.h] type[include]]
dependency[button.o]
#endif

#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class GuiContainer;

class Button:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onActionPerform(Button& source)
					{}
			};

		static Button* create(GuiContainer& parent,const char* title)
			{return create(parent,s_default_handler,0,title);}

		static Button* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title);

		virtual unsigned int idGet() const=0;

		virtual void titleSet(const char* title)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
