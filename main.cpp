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
#include "anjainfo.h"
#include "framework/window.h"
#include "framework/programinfo.h"
#include <cstring>

static constexpr unsigned int LIGHT=0x1;
static constexpr unsigned int FULLSCREEN=0x2;
static constexpr unsigned int HELP_SHOW=0x4;
static constexpr unsigned int ABOUT_SHOW=0x8;

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
			if(strcmp(argv[k],"--help")==0)
				{flags|=HELP_SHOW;}
			else
			if(strcmp(argv[k],"--about")==0)
				{flags|=ABOUT_SHOW;}
			else
				{throw "Unknown option. Try anja --help";}
			}

		if(flags&HELP_SHOW)
			{
			printf("Usage: anja [OPTIONS] [--] FILE\n\n"
				"OPTIONS can be any of the following flags:\n\n"
				"    --about\n"
				"          prints an about message to standard output and exits\n\n"
				"    --dark\n"
				"          starts Anja in dark mode. This is default\n\n"
				"    --fullscreen\n"
				"          starts Anja in fullscreen mode\n\n"
				"    --light\n"
				"          starts Anja in light mode\n\n"
				"    --windowed\n"
				"          starts Anja in windowed mode. This is default\n\n"
				"    --help\n"
				"          prints this message to standard output and exits\n\n"
				"For more information, see Anja user's guide.\n");
			return 0;
			}

		if(flags&ABOUT_SHOW)
			{
			printf(
				"%s---%s\n"
				"version %s\n\n"
				"Written %u by\n"
				,ANJAINFO.name,ANJAINFO.description
				,ANJAINFO.version
				,ANJAINFO.year);

				{
				auto ptr=ANJAINFO.authors;
				while(*ptr!=nullptr)
					{
					printf("    %s\n",*ptr);
					++ptr;
					}
				}

			printf("\nAcknowledgement\n");
				{
				auto ptr=ANJAINFO.acknowledgement;
				while(*ptr!=nullptr)
					{
					printf("    %s\n",*ptr);
					++ptr;
					}
				}

			printf("\n%s\n\nThis Anja was compiled ",ANJAINFO.disclaimer);
				{
				auto ptr=ANJAINFO.compileinfo;
				while(*ptr!=nullptr)
					{
					printf("%s",*ptr);
					++ptr;
					}
				}

			return 0;
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
