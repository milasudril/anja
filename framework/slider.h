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
			,const char* title,unsigned int id)
			{return create(parent,handler,title,0,id);}

		static Slider* create(GuiContainer& parent,const char* title
			,unsigned int id,bool horizontal)
			{return create(parent,s_default_handler,title,id,horizontal);}

		static Slider* create(GuiContainer& parent,EventHandler& handler
			,const char* title,unsigned int id,bool horizontal);
	};
#endif

