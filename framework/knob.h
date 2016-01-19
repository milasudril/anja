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
		static Knob* create(GuiContainer& parent,const char* title)
			{return create(parent,s_default_handler,0,title);}

		static Knob* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title);
	};
#endif

