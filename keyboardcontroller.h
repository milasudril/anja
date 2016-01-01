#ifdef __WAND__
target[name[keyboardcontroller.h] type[include]]
dependency[keyboardcontroller.o]
#endif

#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "framework/keyboardview.h"
#include "framework/array_fixed.h"

class AudioConnection;
class SessionView;

class KeyboardController:public KeyboardView::EventHandler
	{
	public:
		KeyboardController():r_view(nullptr),r_connection(nullptr){}

		void onMouseUp(KeyboardView& source,uint8_t scancode,keymask_t key_mask);

		void onKeyDown(KeyboardView& source,uint8_t scancode);

		void onKeyUp(KeyboardView& source,uint8_t scancode);

		void audioConnectionSet(AudioConnection* connection)
			{r_connection=connection;}

		void sessionViewSet(SessionView* view)
			{r_view=view;}

	private:
		SessionView* r_view;
		AudioConnection* r_connection;
		ArrayFixed<bool,256> m_keystates;
	};

#endif
