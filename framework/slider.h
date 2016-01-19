#ifdef __WAND__
target[name[slider.h] type[include]]
dependency[slider.o]
#endif

#ifndef SLIDER_H
#define SLIDER_H

#include "valueinput.h"

class Slider:public ValueInput
	{
	public:
		static Slider* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title)
			{return create(parent,handler,id,title,0);}

		static Slider* create(GuiContainer& parent,const char* title,bool horizontal)
			{return create(parent,s_default_handler,0,title,horizontal);}

		static Slider* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id,const char* title,bool horizontal);
	};
#endif

