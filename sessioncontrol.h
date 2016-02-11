#ifdef __WAND__
target[name[sessioncontrol.h] type[include]]
dependency[sessioncontrol.o]
#endif

#ifndef SESSIONCONTROL_H
#define SESSIONCONTROL_H

#include "sessionactions.h"

class SessionView;
class Window;

class SessionControl:public SessionActions::EventHandler
	{
	public:
		SessionControl(Window& window);

		void onSessionNew(SessionActions& source);
		void onSessionLoad(SessionActions& source,const char* filename);
		void onSessionSave(SessionActions& source);
		void onSessionSaveAs(SessionActions& source,const char* filename);
		void onEngineStart(SessionActions& source);
		void onEngineStop(SessionActions& source);
		void onFullscreen(SessionActions& source);
		void onExit(SessionActions& source);

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		SessionView* r_view;
		Window* r_window;
	};

#endif
