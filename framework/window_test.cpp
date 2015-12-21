#ifdef __WAND__
target[name[window_test] type[application] platform[;GNU/Linux]]
#endif

#include "window.h"
#include "eventloop.h"

int main(int argc,char* argv[])
	{
	auto event_loop=EventLoop::create();

	auto window_a=Window::create(*event_loop);
	window_a->titleSet("Window A");

	auto window_b=Window::create(*window_a);
	window_b->titleSet("Window B");

	auto window_c=Window::create(*window_b);
	window_c->titleSet("Window C");

	return 0;
	}
