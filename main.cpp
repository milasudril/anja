#ifdef __WAND__
target[name[anja] type[application]]
#endif


#include "session.h"
#include "sessionview.h"
#include "keyboardcontroller.h"
#include "waveformrangetrimmer.h"
#include "waveformdataupdater.h"
#include "framework/window.h"

int main()
	{
	try
		{
		Session session("testbank/testbank.txt");
		WaveformRangeTrimmer trimmer;
		KeyboardController keyboardevents(session);
		WaveformDataUpdater waveform_updater;

		auto event_loop=EventLoop::create();
		auto mainwin=Window::create(*event_loop);

		ArrayDynamicShort<char> title("Anja - ");
		title.truncate().append(session.titleGet());
		mainwin->titleSet( title.begin() );

		auto view=SessionView::create(*mainwin,session,keyboardevents,waveform_updater
			,trimmer);
		keyboardevents.sessionViewSet(view);
		waveform_updater.sessionViewSet(view);
		}
	catch(const char* msg)
		{printf("Error: %s\n",msg);}
	return 0;
	}
