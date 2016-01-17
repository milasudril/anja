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
		static Textbox* create(GuiContainer& parent,unsigned int element_id);
		virtual const char* textGet() const=0;
		virtual void textSet(const char* text)=0;
		virtual void widthMinSet(unsigned int nChars)=0;
	};

#endif
