#ifdef __WAND__
target[name[keyboardcontroller.h] type[include]]
dependency[keyboardcontroller.o]
#endif

#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "framework/keyboardview.h"
#include "framework/keyboardlayout.h"
#include "framework/array_fixed.h"

class SessionView;
class Session;

class KeyboardController:public KeyboardView::EventHandler
	{
	public:
		KeyboardController(Session& session);

		void onMouseUp(KeyboardView& source,uint8_t scancode,keymask_t key_mask);

		void onKeyDown(KeyboardView& source,uint8_t scancode);

		void onKeyUp(KeyboardView& source,uint8_t scancode);

		void sessionSet(Session& session)
			{r_session=&session;}

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		Session* r_session;
		SessionView* r_view;
		ArrayFixed<bool,256> m_keystates;
	};

#endif
