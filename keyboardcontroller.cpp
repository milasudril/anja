#ifdef __WAND__
target[name[keyboardcontroller.o] type[object]]
#endif

#include "keyboardcontroller.h"
#include "sessionview.h"
#include "session.h"
#include "audioengineanja.h"
#include "midiconstants/statuscodes.h"

#include <cstring>

KeyboardController::KeyboardController(Session& session):
	r_session(&session),r_view(nullptr)
	{
	memset(m_keystates.begin(),0,256*sizeof(uint8_t));
	}

void KeyboardController::onKeyDown(KeyboardView& source,uint8_t scancode)
	{
	auto keystate=m_keystates[scancode];
	auto& engine=r_session->audioEngineGet();
	if(keystate==0)
		{
		m_keystates[scancode]=1;
		auto slot=r_session->scancodeToSlot(scancode);
		if(slot!=255)
			{
			engine.eventPost(MIDIConstants::StatusCodes::NOTE_ON,slot,1.0f);
			}
		}
	}

void KeyboardController::onKeyUp(KeyboardView& source,uint8_t scancode)
	{
	m_keystates[scancode]=0;
	auto& engine=r_session->audioEngineGet();
	auto slot=r_session->scancodeToSlot(scancode);
	if(slot!=255)
		{
		engine.eventPost(MIDIConstants::StatusCodes::NOTE_OFF,slot,1.0f);
		}
	}

void KeyboardController::onMouseUp(KeyboardView& source,uint8_t scancode
	,keymask_t key_mask)
	{
	auto slot=r_session->scancodeToSlot(scancode);
	if(slot!=255)
		{
		r_session->slotActiveSet(slot);
		r_view->slotDisplay(slot);
		source.update();
		}
	}
