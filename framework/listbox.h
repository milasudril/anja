#ifdef __WAND__
target[name[listbox.h] type[include]]
dependency[listbox.o]
#endif

#ifndef LISTBOX_H
#define LISTBOX_H

#include "widget.h"

class GuiContainer;

class Listbox:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onOptionSelect(Listbox& source)
					{}
			};

		static Listbox* create(GuiContainer& parent)
			{return create(parent,s_default_handler,0);}

		static Listbox* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id);

		virtual unsigned int idGet() const=0;
		virtual void optionAppend(const char* option)=0;
		virtual void optionReplace(unsigned int index,const char* option)=0;
		virtual void optionSelect(unsigned int index)=0;
		virtual unsigned int optionSelectedGet() const=0;
		virtual void optionsClear()=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
