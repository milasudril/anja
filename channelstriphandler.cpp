#ifdef __WAND__
target[name[channelstriphandler.o] type[object]]
#endif

#include "channelstriphandler.h"
#include "framework/window.h"
#include "framework/floatconv.h"

#include <cstring>

ChannelStripHandler::ChannelStripHandler(EventHandler& handler):
	r_handler(&handler),m_colordlg(nullptr),m_color_presets(64)
	{
	memcpy(m_color_presets.begin(),COLORS
		,std::min(int(ColorID::COLOR_END),64)*sizeof(ColorRGBA));
	}

ChannelStripHandler::~ChannelStripHandler()
	{
	if(m_colordlg!=nullptr)
		{
		m_picker->destroy();
		m_colordlg->destroy();
		}
	}


double ChannelStripHandler::valueMap(ValueInput& source,double x) const noexcept
	{
	switch(source.idGet()>>4)
		{
		case 0:
			return 1e-3*exp2(12.0*x);
		case 1:
			return 72.0*(x - 1.0) + 6.0*x;
		}
	return x;
	}

double ChannelStripHandler::valueMapInverse(ValueInput& source,double y) const noexcept
	{
	switch(source.idGet()>>4)
		{
		case 0:
			return log2(y*1e3)/12.0;
		case 1:
			return (y+72.0)/78.0;
		}
	return y;
	}

double ChannelStripHandler::valueGet(ValueInput& source,const char* text)
	{
	auto value=convert(text);
	switch(source.idGet()>>4)
		{
		case 0:
			r_handler->onFadeTimeChange(source.idGet()&0xf,value);
			break;
		case 1:
			r_handler->onGainChange(source.idGet()&0xf,value);
			break;
		}
	return value;
	}

void ChannelStripHandler::textGet(ValueInput& source,double value,TextBuffer& buffer)
	{
	switch(source.idGet()>>4)
		{
		case 0:
			r_handler->onFadeTimeChange(source.idGet()&0xf,value);
			sprintf(buffer.begin(),"%.3f",value);
			return;
		case 1:
			r_handler->onGainChange(source.idGet()&0xf,value);
			sprintf(buffer.begin(),"%.3f",value);
			return;
		}
	sprintf(buffer.begin(),"%.3f",value);
	}

void ChannelStripHandler::onLeave(Textbox& source)
	{
	r_handler->onLabelChange(source.idGet()&0xf,source.textGet());
	}

void ChannelStripHandler::onActionPerform(ColorView& source)
	{
	if(m_colordlg==nullptr)
		{
		m_colordlg=Window::create(source,reinterpret_cast<void**>(&m_colordlg));
		m_colordlg->titleSet("Choose a color");
		m_color=source.colorGet();
		m_picker=ColorPicker::create(*m_colordlg
			,m_color
			,m_color_presets.begin(),m_color_presets.length(),*this);
		m_id_active=source.idGet()&0xf;
		}
	}

void ChannelStripHandler::onConfirmed(ColorPicker::Tag tag)
	{
	r_handler->onColorChange(m_id_active,m_color);
	}


