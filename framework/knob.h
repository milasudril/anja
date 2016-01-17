#ifdef __WAND__
target[name[knob.h] type[include]]
dependency[knob.o]
#endif

#ifndef KNOB_H
#define KNOB_H

#include "valueinput.h"

class Knob:public ValueInput
	{
	public:
		static Knob* create(GuiContainer& parent,const char* title
			,unsigned int id)
			{return create(parent,s_default_handler,title,id);}

		static Knob* create(GuiContainer& parent,EventHandler& handler
			,const char* title,unsigned int id);
	};
#endif

