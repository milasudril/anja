#ifdef __WAND__
target[name[knob_test] type[application] platform[;GNU/Linux]]
#endif

#include "knob.h"
#include "window.h"
#include "boxhorizontal.h"

int main()
	{
	auto event_loop=EventLoop::create(0);
	auto mainwin=Window::create(*event_loop);
	Knob::create(*mainwin,"Level");

	return 0;
	}
