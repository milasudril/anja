#ifdef __WAND__
target[name[colorpicker_test] type[application] platform[;GNU/Linux]]
#endif

#include "colorpicker.h"
#include "color.h"
#include "window.h"
#include "array_simple.h"
#include <cstring>

int main()
	{
	ColorRGBA color;
	ArraySimple<ColorRGBA> presets(ColorID::COLOR_END);
	memcpy(presets.begin(),COLORS,ColorID::COLOR_END*sizeof(ColorRGBA));

	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto picker=ColorPicker::create(*mainwin,color,presets.begin(),ColorID::COLOR_END);
	return 0;
	}
