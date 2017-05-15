//@	{
//@	 "targets":[{"name":"channelstrip.o","type":"object"}]
//@	}

#include "channelstrip.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;


ChannelStrip& ChannelStrip::colorPresets(const ColorRGBA* begin,const ColorRGBA* end)
	{
	r_color_presets_begin=begin;
	r_color_presets_end=end;
	if(m_color_dlg)
		{
		if(m_color_dlg)
			{m_color_dlg->widget().presets(begin,end);}
		}
	return *this;
	}


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

static void fade_time_update(const ChannelView& channel,TextEntry& e,Knob& k)
	{
	auto t=channel.fadeTimeGet();
	char buffer[16];
	sprintf(buffer,"%.3f",t);
	e.content(buffer);
	k.value(t);
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
			if(r_cb_obj!=nullptr)
				{m_vtable.name_changed(r_cb_obj,*this,m_id);}
			label_update(m_channel,entry);
			break;

		case TextEntryId::FADETIME:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_channel.fadeTimeSet(val_new);}
			fade_time_update(m_channel,entry,m_ft_knob);
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
				.color(m_channel.colorGet())
				.presets(r_color_presets_begin,r_color_presets_end);
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

void ChannelStrip::changed(Knob& knob,KnobId id)
	{
	switch(id)
		{
		case KnobId::FADETIME:
			
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
			if(r_cb_obj!=nullptr)
				{
				m_vtable.color_changed(r_cb_obj,*this,m_id);
				m_vtable.color_presets_changed(r_cb_obj,dlg.widget());
				}
			color_update(m_channel,m_color);
			m_color_dlg.reset();
			break;
		}
	}


ChannelStrip::ChannelStrip(Container& cnt,const ChannelView& channel):
	 r_cb_obj(nullptr)
	,m_channel(channel)
	,m_box(cnt,true)
		,m_name(m_box)
		,m_color(m_box)
		,m_sep_a(m_box.insertMode({4,0}),false)
		,m_ft_label(m_box.insertMode({0,0}),"FT/s")
		,m_ft_knob(m_box.insertMode({4,0}))
		,m_ft_input(m_box.insertMode({0,0}))
		,m_sep_b(m_box.insertMode({4,0}),false)
		,m_gain_slider(m_box.insertMode({0,Box::EXPAND|Box::FILL}),true)
		,m_gain_input(m_box.insertMode({0,0}))
	{
	m_name.small(1).width(7);
	m_ft_input.small(1).width(7);
	m_gain_input.small(1).width(7);

	m_name.callback(*this,TextEntryId::LABEL);
	m_color.callback(*this,ColorViewId::COLOR);
	m_ft_knob.callback(*this,KnobId::FADETIME);
	m_ft_input.callback(*this,TextEntryId::FADETIME);
	m_gain_slider.callback(*this,SliderId::GAIN);
	m_gain_input.callback(*this,TextEntryId::GAIN);


	label_update(channel,m_name);
	color_update(channel,m_color);
	fade_time_update(channel,m_ft_input,m_ft_knob);
	gain_update(channel,m_gain_slider,m_gain_input);
	}
