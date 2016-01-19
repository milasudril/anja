#ifdef __WAND__
target[name[textbox.h] type[include]]
dependency[textbox.o]
#endif

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "widget.h"

class GuiContainer;

class Textbox:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onLeave(Textbox& source)
					{}
			};

		static Textbox* create(GuiContainer& parent,unsigned int id)
			{return create(parent,s_default_handler,id);}

		static Textbox* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id);

		virtual const char* textGet() const=0;
		virtual void textSet(const char* text)=0;
		virtual void widthMinSet(unsigned int nChars)=0;
		virtual unsigned int idGet() const=0;

	private:
		static EventHandler s_default_handler;
	};

#endif
