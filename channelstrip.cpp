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
#include "framework/floatconv.h"
#include "framework/window.h"
#include <cmath>

#include <cstring>

ChannelStrip* ChannelStrip::create(GuiContainer& parent,EventHandler& handler
	,unsigned int id)
	{return new ChannelStrip(parent,handler,id);}

void ChannelStrip::destroy()
	{
	delete this;
	}

const GuiHandle& ChannelStrip::handleNativeGet() const
	{return m_box->handleNativeGet();}

ChannelStrip::ChannelStrip(GuiContainer& parent,EventHandler& handler
	,unsigned int id):
	r_handler(&handler),m_input_handler(*this),m_id(id)
	{
	m_box=BoxVertical::create(parent);
	m_box->slaveAssign(*this);

	m_label=Textbox::create(*m_box,m_input_handler,0);
	m_label->widthMinSet(5);
	m_color=ColorView::create(*m_box,m_input_handler,0);
	m_fadetime=Knob::create(*m_box,m_input_handler,0,"Fade\ntime/s");

	m_box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL
		|BoxVertical::INSERTMODE_END);
	m_level=Slider::create(*m_box,m_input_handler,1,"Gain\n/dB",0);

	}

ChannelStrip::~ChannelStrip()
	{
	m_level->destroy();
	m_fadetime->destroy();
	m_label->destroy();
	m_box->slaveRelease();
	m_box->destroy();
	}



ChannelStrip::ValueInputHandler::ValueInputHandler(ChannelStrip& strip):
	r_strip(strip),m_colordlg(nullptr),m_color_presets(64)
	{
	memcpy(m_color_presets.begin(),COLORS
		,std::min(int(ColorID::COLOR_END),64)*sizeof(ColorRGBA));
	}

ChannelStrip::ValueInputHandler::~ValueInputHandler()
	{
	if(m_colordlg!=nullptr)
		{
		m_picker->destroy();
		m_colordlg->destroy();
		}
	}


double ChannelStrip::ValueInputHandler::valueMap(ValueInput& source,double x) const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return 1e-3*exp2(12.0*x);
		case 1:
			return 72.0*(x - 1.0) + 6.0*x;
		}
	return x;
	}

double ChannelStrip::ValueInputHandler::valueMapInverse(ValueInput& source,double y) const noexcept
	{
	switch(source.idGet())
		{
		case 0:
			return log2(y*1e3)/12.0;
		case 1:
			return (y+72.0)/78.0;
		}
	return y;
	}

double ChannelStrip::ValueInputHandler::valueGet(ValueInput& source,const char* text)
	{
	auto value=convert(text);
	switch(source.idGet())
		{
		case 0:
			r_strip.doFadeTimeChange(value);
			break;
		case 1:
			r_strip.doGainChange(value);
			break;
		}
	return value;
	}

void ChannelStrip::ValueInputHandler::textGet(ValueInput& source,double value,TextBuffer& buffer)
	{
	switch(source.idGet())
		{
		case 0:
			r_strip.doFadeTimeChange(value);
			sprintf(buffer.begin(),"%.3f",value);
			return;
		case 1:
			r_strip.doGainChange(value);
			sprintf(buffer.begin(),"%.3f",value);
			return;
		}
	sprintf(buffer.begin(),"%.3f",value);
	}

void ChannelStrip::ValueInputHandler::onLeave(Textbox& source)
	{
	r_strip.doLabelChange(source.textGet());
	}

void ChannelStrip::ValueInputHandler::onActionPerform(ColorView& source)
	{
	if(m_colordlg==nullptr)
		{
		m_colordlg=Window::create(source,reinterpret_cast<void**>(&m_colordlg));
		m_colordlg->titleSet("Choose a color");
		m_color=source.colorGet();
		m_picker=ColorPicker::create(*m_colordlg
			,m_color
			,m_color_presets.begin(),m_color_presets.length(),*this);
		}
	}

void ChannelStrip::ValueInputHandler::onConfirmed(ColorPicker::Tag tag)
	{
	r_strip.doColorChange(m_color);
	}


void ChannelStrip::channelDataSet(const ChannelData& data)
	{
	m_label->textSet(data.labelGet().begin());
	const auto& channel=data.channelGet();
	m_fadetime->valueSet(channel.fadeTimeGet());
	m_level->valueSet(channel.gainGet());
	m_color->colorSet(data.colorGet());
	}
