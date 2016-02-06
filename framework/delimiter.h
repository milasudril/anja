#ifdef __WAND__
target[name[delimiter.h] type[include]]
dependency[delimiter.o]
#endif

#ifndef DELIMITER_H
#define DELIMITER_H

#include "widget.h"

class BoxVertical;
class BoxHorizontal;
class GuiContainer;

class Delimiter:public Widget
	{
	public:
		static Delimiter* create(BoxVertical& parent);
		static Delimiter* create(BoxHorizontal& parent);

	private:
		static Delimiter* create(GuiContainer& parent,bool horizontal);
	};

#endif
