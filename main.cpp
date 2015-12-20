#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "framework/window.h"
#include "waveformview.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");
	auto waveformview=WaveformView::create(*mainwin);
	return 0;
	}
