#ifdef __WAND__
target[name[checkbox.h] type[include]]
dependency[checkbox.o]
#endif

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "widget.h"

class GuiContainer;

class Checkbox:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onSet(Checkbox& source)
					{}

				virtual void onUnset(Checkbox& source)
					{}
			};

		static Checkbox* create(GuiContainer& parent,const char* title)
			{return create(parent,s_default_handler,0,title);}

		static Checkbox* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title);

		virtual bool stateGet() const=0;
		virtual void stateSet(bool state_new)=0;
		virtual unsigned int idGet() const=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
