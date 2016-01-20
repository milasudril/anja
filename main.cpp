#ifdef __WAND__
target[name[anja] type[application]]
#endif


#include "session.h"
#include "sessionview.h"
#include "keyboardcontroller.h"
#include "channelcontroller.h"
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
		ChannelController channelctrl(session);

		auto event_loop=EventLoop::create(0);
		auto mainwin=Window::create(*event_loop);

		ArrayDynamicShort<char> title("Anja - ");
		title.truncate().append(session.titleGet());
		mainwin->titleSet( title.begin() );

		auto view=SessionView::create(*mainwin,session,keyboardevents
			,waveform_updater,trimmer,channelctrl);
		keyboardevents.sessionViewSet(view);
		waveform_updater.sessionViewSet(view);
		channelctrl.sessionViewSet(view);
		}
	catch(const char* msg)
		{printf("Error: %s\n",msg);}
	return 0;
	}
