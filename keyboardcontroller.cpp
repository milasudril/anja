#ifdef __WAND__
target[name[keyboardcontroller.o] type[object]]
#endif

#include "keyboardcontroller.h"
#include "sessionview.h"
#include "session.h"
#include "audioconnection.h"

#include <cstring>

void KeyboardController::onKeyDown(KeyboardView& source,uint8_t scancode)
	{
	auto keystate=m_keystates[scancode];
	auto connection=r_session->audioConnectionGet();
	if(connection!=nullptr && keystate==0)
		{
		printf("%u\n",scancode);
		m_keystates[scancode]=1;
		connection->eventPost(scancode,0);
		}
	}

void KeyboardController::onKeyUp(KeyboardView& source,uint8_t scancode)
	{
	m_keystates[scancode]=0;
	auto connection=r_session->audioConnectionGet();
	if(connection!=nullptr)
		{
		connection->eventPost(scancode,1);
		}
	}

void KeyboardController::onMouseUp(KeyboardView& source,uint8_t scancode
	,keymask_t key_mask)
	{
	auto slot=r_session->scancodeToSlot(scancode);
	r_session->slotActiveSet(slot);
	r_view->slotDisplay(slot);
	source.update();
	}
