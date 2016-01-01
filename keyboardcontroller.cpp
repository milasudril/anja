#ifdef __WAND__
target[name[keyboardcontroller.o] type[object]]
#endif

#include "keyboardcontroller.h"
#include "sessionview.h"
#include "audioconnection.h"

void KeyboardController::onKeyDown(KeyboardView& source,uint8_t scancode)
	{
	auto keystate=m_keystates[scancode];
	if(r_connection!=nullptr && keystate==0)
		{
		printf("%u\n",scancode);
		m_keystates[scancode]=1;
		r_connection->eventPost(scancode,0);
		}
	}

void KeyboardController::onKeyUp(KeyboardView& source,uint8_t scancode)
	{
	m_keystates[scancode]=0;
	if(r_connection!=nullptr)
		{
		r_connection->eventPost(scancode,1);
		}
	}

void KeyboardController::onMouseUp(KeyboardView& source,uint8_t scancode
	,keymask_t key_mask)
	{
	r_view->slotDisplayFromScancode(scancode);
	source.update();
	}
