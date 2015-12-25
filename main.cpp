#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "waveformdataview.h"
#include "waveformdata.h"
#include "waveformrangetrimmer.h"
#include "session.h"

#include "framework/window.h"
#include "framework/boxvertical.h"
#include "framework/keyboardlayout.h"
#include "framework/keyboardview.h"

#include <cmath>

int main()
	{
	KeyboardLayout layout;
	WaveformRangeTrimmer trimmer;
	Session session;

	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");

	auto box=BoxVertical::create(*mainwin);
	box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);
	auto keyview=KeyboardView::create(*box,layout);


	box->insertModeSet(BoxVertical::INSERTMODE_END);
	auto waveformdata=WaveformDataView::create(*box,trimmer);

	return 0;
	}
