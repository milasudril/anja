#ifdef __WAND__
target[name[mainwinhandler.h] type[include]]
dependency[mainwinhandler.o]
#endif

#include "framework/window.h"

#ifndef MAINWINHANDLER_H
#define MAINWINHANDLER_H

class SessionActions;

class MainwinHandler:public Window::EventHandler
	{
	public:
		MainwinHandler():r_actions(nullptr){}

		void onClose(Window& source);

		void sessionActionsSet(SessionActions* actions)
			{r_actions=actions;}

	private:
		SessionActions* r_actions;
	};

#endif
