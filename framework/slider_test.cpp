#ifdef __WAND__
target[name[slider_test] type[application] platform[;GNU/Linux]]
#endif

#include "slider.h"
#include "window.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto slider=Slider::create(*mainwin,true);
	return 0;
	}
