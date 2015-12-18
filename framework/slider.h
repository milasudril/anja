#ifdef __WAND__
target[name[slider.h] type[include]]
dependency[slider.o]
#endif

#ifndef SLIDER_H
#define SLIDER_H

class GuiContainer;

#include <cstddef>

template<class T,size_t N>
class ArrayFixed;

class Slider
	{
	public:
		class EventHandler
			{
			public:
				typedef ArrayFixed<char,32> TextBuffer;

				virtual void textGet(double value, TextBuffer& buffer);
				virtual double valueGet(const char* text);
			};

		static Slider* create(GuiContainer& parent)
			{return create(parent,s_null_handler);}

		static Slider* create(GuiContainer& parent,EventHandler& handler);

		virtual void destroy()=0;
		virtual void valueSet(double value)=0;

	protected:
		virtual ~Slider()=default;
	private:
		static EventHandler s_null_handler;
	};

#endif

