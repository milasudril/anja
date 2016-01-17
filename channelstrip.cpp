#ifdef __WAND__
target[name[channelstrip.o] type[object]]
#endif

#include "channelstrip.h"
#include "channeldata.h"
#include "channel.h"
#include "framework/boxvertical.h"
#include "framework/textbox.h"
#include "framework/knob.h"
#include "framework/slider.h"
#include <cmath>

ChannelStrip* ChannelStrip::create(GuiContainer& parent)
	{return new ChannelStrip(parent);}

void ChannelStrip::destroy()
	{
	delete this;
	}

const GuiHandle& ChannelStrip::handleNativeGet() const
	{return m_box->handleNativeGet();}

ChannelStrip::ChannelStrip(GuiContainer& parent):m_input_handler(*this)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);

	m_label=Textbox::create(*m_box,0);
	m_label->widthMinSet(5);
	m_fadetime=Knob::create(*m_box,m_input_handler,"Fade\ntime/s",0);

	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL
		|BoxVertical::INSERTMODE_END);
	m_level=Slider::create(*m_box,m_input_handler,"Gain\n/dB",1,0);

	}

ChannelStrip::~ChannelStrip()
	{
	m_level->destroy();
	m_fadetime->destroy();
	m_label->destroy();
	m_box->slaveRelease();
	}

double ChannelStrip::ValueInputHandler::valueMap(ValueInput& source,double x) const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return 1e-3*exp2(12.0*x);
		case 1:
			return 120.0*(x - 1.0) + 6.0*x;
		}
	}

double ChannelStrip::ValueInputHandler::valueMapInverse(ValueInput& source,double y) const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return log2(y*1e3)/12.0;
		default:
			return (y+120.0)/126;;
		}
	}

void ChannelStrip::update()
	{
	m_label->textSet(r_channel->labelGet().begin());
	auto& channel=r_channel->channelGet();
	m_fadetime->valueSet(channel.fadeTimeGet());
	m_level->valueSet(channel.gainGet());
	}
