#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "framework/window.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja");
	return 0;
	}
