#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "session.h"
#include "sessionview.h"
#include "keyboardcontroller.h"
#include "channelcontroller.h"
#include "waveformrangetrimmer.h"
#include "waveformdataupdater.h"
#include "sessiondataupdater.h"
#include "sessioncontrol.h"
#include "mainwinhandler.h"
#include "framework/window.h"
#include <cstring>

static constexpr unsigned int LIGHT=0x1;
static constexpr unsigned int FULLSCREEN=0x2;

int main(int argc,char* argv[])
	{
	try
		{
		Session session;

		unsigned int flags=0;
		int k;
		for(k=1;k<argc;++k)
			{
			if(strcmp(argv[k],"--light")==0)
				{flags|=LIGHT;}
			else
			if(strcmp(argv[k],"--dark")==0)
				{flags&=~LIGHT;}
			else
			if(strcmp(argv[k],"--fullscreen")==0)
				{flags|=FULLSCREEN;}
			else
			if(strcmp(argv[k],"--windowed")==0)
				{flags&=~FULLSCREEN;}
			else
			if(strcmp(argv[k],"--")==0)
				{
				++k;
				break;
				}
			else
				{throw "Unknown option";}
			}

		if(k<argc)
			{
			try
				{
				session.load(argv[k]);
				--argc;
				}
			catch(...)
				{}
			}
		WaveformRangeTrimmer trimmer;
		KeyboardController keyboardevents(session);
		WaveformDataUpdater waveform_updater;
		ChannelController channelctrl(session);
		SessionDataUpdater session_updater;
		MainwinHandler mainwin_handler;


		auto event_loop=EventLoop::create(flags&LIGHT);
		auto mainwin=Window::create(*event_loop,mainwin_handler);
		SessionControl session_control(*mainwin);

		auto view=SessionView::create(*mainwin,session,session_control
			,keyboardevents,waveform_updater,trimmer,channelctrl
			,session_updater);

		mainwin_handler.sessionActionsSet(view->actionsGet());
		keyboardevents.sessionViewSet(view);
		waveform_updater.sessionViewSet(view);
		channelctrl.sessionViewSet(view);
		session_updater.sessionViewSet(view);
		session_control.sessionViewSet(view);

		if(flags&FULLSCREEN)
			{session_control.onFullscreen(*view->actionsGet());}
		}
	catch(const char* msg)
		{printf("Error: %s\n",msg);}
	return 0;
	}
