#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "framework/window.h"
#include "framework/boxvertical.h"
#include "framework/keyboardview.h"
#include "framework/keyboardlayout.h"
#include "framework/xyplot.h"

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");
	auto panels=BoxVertical::create(*mainwin);
	panels->insertModeSet(BoxVertical::INSERTMODE_FILL | BoxVertical::INSERTMODE_EXPAND);
	KeyboardLayout keyboard;
	auto keyboardview=KeyboardView::create(*panels,keyboard);
	panels->insertModeSet(BoxVertical::INSERTMODE_END | BoxVertical::INSERTMODE_FILL | BoxVertical::INSERTMODE_EXPAND);
	auto xyplot=XYPlot::create(*panels);
	return 0;
	}
