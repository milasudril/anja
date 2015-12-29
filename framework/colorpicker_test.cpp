#ifdef __WAND__
target[name[colorpicker_test] type[application] platform[;GNU/Linux]]
#endif

#include "colorpicker.h"
#include "color.h"
#include "window.h"

int main()
	{
	Color color;
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto picker=ColorPicker::create(*mainwin,color,COLORS,ColorID::COLOR_END);
	return 0;
	}
