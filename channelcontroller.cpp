#ifdef __WAND__
target[name[channelcontroller.o] type[object]]
#endif

#include "channelcontroller.h"
#include "session.h"
#include "sessionview.h"
#include "units.h"
#include "midiconstants/statuscodes.h"
#include "midiconstants/controlcodes.h"

void ChannelController::onLabelChange(unsigned int channel,const char* label)
	{
	r_session->channelDataGet(channel).labelSet(label);
	r_view->channelNameUpdate(channel);
	r_view->keyboardViewUpdate();
	}

void ChannelController::onFadeTimeChange(unsigned int channel,float time)
	{
	r_session->channelGet(channel).fadeTimeSet(time);
	}

void ChannelController::onGainChange(unsigned int channel,float value)
	{
	r_session->channelGet(channel).gainSet(value);
	r_session->audioEngineGet().eventPost(channel|MIDIConstants::StatusCodes::CONTROLLER
		,MIDIConstants::ControlCodes::CHANNEL_VOLUME
		,dBToAmplitude(value));
	}

void ChannelController::onColorChange(unsigned int channel,const ColorRGBA& color)
	{
	r_session->channelDataGet(channel).colorSet(color);
	r_view->channelColorUpdate(channel);
	r_view->keyboardViewUpdate();
	}
