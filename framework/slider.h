#ifdef __WAND__
target[name[slider.h] type[include]]
dependency[slider.o]
#endif

#ifndef SLIDER_H
#define SLIDER_H

class GuiContainer;

#include "widget.h"
#include <cstddef>

template<class T,size_t N>
class ArrayFixed;

class Slider:public Widget
	{
	public:
		class EventHandler
			{
			public:
				typedef ArrayFixed<char,32> TextBuffer;

				virtual void textGet(Slider& source,double value, TextBuffer& buffer);
				virtual double valueGet(Slider& source,const char* text);
				virtual double valueMap(Slider& source,double x) const noexcept;
				virtual double valueMapInverse(Slider& source,double y) const noexcept;
			};

		static Slider* create(GuiContainer& parent)
			{return create(parent,s_default_handler);}

		static Slider* create(GuiContainer& parent,EventHandler& handler)
			{return create(parent,handler,0);}

		static Slider* create(GuiContainer& parent,bool horizontal)
			{return create(parent,s_default_handler,horizontal);}

		static Slider* create(GuiContainer& parent,EventHandler& handler,bool horizontal);

		virtual void valueSet(double y)=0;

	private:
		static EventHandler s_default_handler;
	};

#endif

