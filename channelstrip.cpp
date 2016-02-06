#ifdef __WAND__
target[name[channelstrip.o] type[object]]
#endif

#include "channelstrip.h"
#include "channeldata.h"
#include "channel.h"
#include "channelstriphandler.h"

#include "framework/boxvertical.h"
#include "framework/textbox.h"
#include "framework/knob.h"
#include "framework/slider.h"
#include "framework/floatconv.h"
#include "framework/window.h"
#include <cmath>

#include <cstring>

ChannelStrip* ChannelStrip::create(GuiContainer& parent,ChannelStripHandler& handler
	,unsigned int id)
	{return new ChannelStrip(parent,handler,id);}

void ChannelStrip::destroy()
	{
	delete this;
	}

const GuiHandle& ChannelStrip::handleNativeGet() const
	{return m_box->handleNativeGet();}

ChannelStrip::ChannelStrip(GuiContainer& parent,ChannelStripHandler& handler
	,unsigned int id):m_id(id)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);

	m_label=Textbox::create(*m_box,handler,id);
	m_label->widthMinSet(5);
	m_color=ColorView::create(*m_box,handler,id);
	m_fadetime=Knob::create(*m_box,handler,id,"Fade\ntime/s");

	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL
		|BoxVertical::INSERTMODE_END);
	m_level=Slider::create(*m_box,handler,id|(1<<4),"Gain\n/dB",0);

	}

ChannelStrip::~ChannelStrip()
	{
	m_level->destroy();
	m_fadetime->destroy();
	m_label->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}

void ChannelStrip::channelDataSet(const ChannelData& data)
	{
	m_label->textSet(data.labelGet().begin());
	const auto& channel=data.channelGet();
	m_fadetime->valueSet(channel.fadeTimeGet());
	m_level->valueSet(channel.gainGet());
	m_color->colorSet(data.colorGet());
	}
