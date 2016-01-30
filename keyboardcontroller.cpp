#ifdef __WAND__
target[name[keyboardcontroller.o] type[object]]
#endif

#include "keyboardcontroller.h"
#include "sessionview.h"
#include "session.h"
#include "audioengineanja.h"
#include "midiconstants/statuscodes.h"
#include "midiconstants/controlcodes.h"

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
		if(slot!=255) //A slot was activated
			{engine.eventPost(MIDIConstants::StatusCodes::NOTE_ON,slot,1.0f);}
		else
		if( (scancode>=59 && scancode <=68) || (scancode>=87 && scancode<=88))
		// function key (Channel fade in/out)
			{
			auto channel=(scancode>=59 && scancode<=68)?
				(scancode - 59) : (scancode - 87 + 10);
			if(m_keystates[108] && !m_keystates[103] && !m_keystates[105]
				&& !m_keystates[106]) // Fade out
				{
				engine.eventPost(
					 MIDIConstants::StatusCodes::CONTROLLER|channel
					,MIDIConstants::ControlCodes::SOUND_1
					,r_session->channelGet(channel).fadeTimeGet());
				}
			if(m_keystates[103] && !m_keystates[108]  && !m_keystates[105]
				&& !m_keystates[106]) // Fade in
				{
				engine.eventPost(
					 MIDIConstants::StatusCodes::CONTROLLER|channel
					,MIDIConstants::ControlCodes::SOUND_2
					,r_session->channelGet(channel).fadeTimeGet());
				}
			if(!m_keystates[108] && !m_keystates[103] && m_keystates[105]
				&& !m_keystates[106]) // Mute
				{
				engine.eventPost(
					 MIDIConstants::StatusCodes::CONTROLLER|channel
					,MIDIConstants::ControlCodes::SOUND_1
					,1e-2f);
				}
			if(!m_keystates[103] && !m_keystates[108]  && !m_keystates[105]
				&& m_keystates[106]) // Unmute
				{
				engine.eventPost(
					 MIDIConstants::StatusCodes::CONTROLLER|channel
					,MIDIConstants::ControlCodes::SOUND_2
					,1e-2f);
				}
			}
		else
		if(scancode==1)
		//	Escape
			{
			engine.eventPost(MIDIConstants::StatusCodes::RESET,0,uint8_t(0));
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
		r_session->keyHighlight(scancode);
		r_view->slotDisplay(slot);
		source.update();
		return;
		}
	slot=r_session->scancodeToChannel(scancode);
	if(slot!=255)
		{
		r_view->channelDisplay(slot);
		r_session->keyHighlight(scancode);
		source.update();
		return;
		}
	}
