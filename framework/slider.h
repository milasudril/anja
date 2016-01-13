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

		static Slider* create(GuiContainer& parent,EventHandler& handler
			,const char* title,unsigned int id)
			{return create(parent,handler,title,0,id);}

		static Slider* create(GuiContainer& parent,const char* title
			,unsigned int id,bool horizontal)
			{return create(parent,s_default_handler,title,id,horizontal);}

		static Slider* create(GuiContainer& parent,EventHandler& handler
			,const char* title,unsigned int id,bool horizontal);

		virtual void valueSet(double y)=0;

		virtual unsigned int idGet() const=0;

	private:
		static EventHandler s_default_handler;
	};

#endif

