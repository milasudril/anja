//@	{
//@	 "targets":[{"name":"channelstrip.o","type":"object"}]
//@	}

#include "channelstrip.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;


static double gain_map(double x)
	{return 6.0*x + (1-x)*(-72.0);}

static double gain_map_inv(double x)
	{return (x + 72)/(72 + 6);}

static void label_update(const ChannelView& channel,TextEntry& e)
	{
	e.content(channel.labelGet().begin());
	}

static void color_update(const ChannelView& channel,ColorView& e)
	{
	e.color(channel.colorGet());
	}

static void fade_time_update(const ChannelView& channel,TextEntry& e)
	{
	auto g=channel.fadeTimeGet();
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
//	s.value(gain_map_inv(g));
	}


static void gain_update(const ChannelView& channel,Slider& s,TextEntry& e)
	{
	auto g=channel.gainGet();
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
	s.value(gain_map_inv(g));
	}


void ChannelStrip::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::LABEL:
			m_channel.labelSet(Anja::String(entry.content()));
			label_update(m_channel,entry);
			break;

		case TextEntryId::FADETIME:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_channel.fadeTimeSet(val_new);}
			fade_time_update(m_channel,entry);
			}
			break;

		case TextEntryId::GAIN:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_channel.gainSet(val_new);}
			gain_update(m_channel,m_gain_slider,entry);
			}
			break;
		}
	}

void ChannelStrip::clicked(ColorView& entry,ColorViewId id)
	{
	switch(id)
		{
		case ColorViewId::COLOR:
			m_color_dlg.reset(new Dialog<ColorPicker>(m_box,"Choose a color"));
			m_color_dlg->callback(*this,PopupId::COLOR_SELECT).widget()
				.color(m_channel.colorGet());
			//	.palette(m_color_presets.begin(),m_color_presets.end());
			break;
		}
	}

void ChannelStrip::changed(Slider& slider,SliderId id)
	{
	switch(id)
		{
		case SliderId::GAIN:
			m_channel.gainSet( gain_map(slider.value()) );
			gain_update(m_channel,slider,m_gain_input);
			break;
		}
	}

void ChannelStrip::dismiss(Dialog<ColorPicker>& dlg,PopupId id)
	{
	switch(id)
		{
		case PopupId::COLOR_SELECT:
			m_color_dlg.reset();
			break;
		}
	}


void ChannelStrip::confirmPositive(Dialog<ColorPicker>& dlg,PopupId id)
	{
	switch(id)
		{
		case PopupId::COLOR_SELECT:
			m_channel.colorSet(dlg.widget().color());
			color_update(m_channel,m_color);
			m_color_dlg.reset();
			break;
		}
	}


ChannelStrip::ChannelStrip(Container& cnt,const ChannelView& channel):
	m_channel(channel)
	,m_box(cnt,true)
		,m_name(m_box)
		,m_color(m_box)
		,m_ft_label(m_box,"Fade\ntime/s")
		,m_ft_input(m_box)
		,m_gain_slider(m_box.insertMode(Box::InsertMode{0,Box::EXPAND|Box::FILL}),true)
		,m_gain_input(m_box.insertMode(Box::InsertMode{0,0}))
	{
	m_name.small(1).width(4);
	m_ft_input.small(1).width(4);
	m_gain_input.small(1).width(4);

	m_name.callback(*this,TextEntryId::LABEL);
	m_color.callback(*this,ColorViewId::COLOR);
	m_ft_input.callback(*this,TextEntryId::FADETIME);
	m_gain_slider.callback(*this,SliderId::GAIN);
	m_gain_input.callback(*this,TextEntryId::GAIN);


	label_update(channel,m_name);
	color_update(channel,m_color);
	fade_time_update(channel,m_ft_input);
	gain_update(channel,m_gain_slider,m_gain_input);
	}
