#ifdef __WAND__
target[name[test2] type[application] platform[;GNU/Linux]]
#endif

#include "slider.h"
#include "window.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto slider=Slider::create(*mainwin,true);
	slider->valueSet(0.5);
	return 0;
	}
