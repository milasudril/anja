#ifdef __WAND__
target[name[inputentry_test] type[application]]
#endif

#include "window.h"
#include "inputentry.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto inputentry=InputEntry::create(*mainwin,"Begin:","Auto");

	return 0;
	}
