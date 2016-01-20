#ifdef __WAND__
target[name[channelcontroller.o] type[object]]
#endif

#include "channelcontroller.h"
#include "session.h"
#include "sessionview.h"
#include "units.h"
#include "midiconstants/statuscodes.h"
#include "midiconstants/controlcodes.h"

void ChannelController::onLabelChange(ChannelStrip& source,const char* label)
	{
	auto ch=source.idGet();
	r_session->channelDataGet(ch).labelSet(label);
	r_view->channelNameUpdate(ch);
	}

void ChannelController::onFadeTimeChange(ChannelStrip& source,float time)
	{
	auto ch=source.idGet();
	r_session->channelGet(ch).fadeTimeSet(ch);
	}

void ChannelController::onGainChange(ChannelStrip& source,float value)
	{
	auto ch=source.idGet();
	r_session->channelGet(ch).fadeTimeSet(ch);
	r_session->audioEngineGet().eventPost(ch|MIDIConstants::StatusCodes::CONTROLLER
		,MIDIConstants::ControlCodes::CHANNEL_VOLUME
		,dBToAmplitude(value));
	}
