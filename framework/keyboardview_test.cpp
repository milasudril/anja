#ifdef __WAND__
target[name[keyboardview_test] type[application] platform[;GNU/Linux]]
#endif

#include "keyboardview.h"
#include "keyboardlayout.h"
#include "window.h"

int main(int argc,char** argv)
	{
	KeyboardLayout keyboard;
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto keyboardview=KeyboardView::create(*mainwin,keyboard);
	return 0;
	}
