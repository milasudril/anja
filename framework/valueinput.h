#ifdef __WAND__
target[name[valueinput.h] type[include]]
dependency[valueinput.o]
#endif


#ifndef VALUEINPUT_H
#define VALUEINPUT_H

class GuiContainer;

#include "widget.h"
#include "array_fixed.h"

class ValueInput:public Widget
	{
	public:
		class EventHandler
			{
			public:
				typedef ArrayFixed<char,32> TextBuffer;

				virtual void textGet(ValueInput& source,double value, TextBuffer& buffer);
				virtual double valueGet(ValueInput& source,const char* text);
				virtual double valueMap(ValueInput& source,double x) const noexcept;
				virtual double valueMapInverse(ValueInput& source,double y) const noexcept;
			};

		virtual void valueSet(double y)=0;

		virtual unsigned int idGet() const=0;

	protected:
		static EventHandler s_default_handler;
	};

#endif

