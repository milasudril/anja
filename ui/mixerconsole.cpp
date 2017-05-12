//@	{"targets":[{"name":"mixerconsole.o","type":"object"}]}

#include "mixerconsole.hpp"
#include "../sessiondata/session.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;


static double gain_map(double x)
	{return 6.0*x + (1-x)*(-72.0);}

static double gain_map_inv(double x)
	{return (x + 72)/(72 + 6);}

MixerConsole& MixerConsole::colorPresets(const ColorRGBA* begin,const ColorRGBA* end)
	{
	std::for_each(m_strips.begin(),m_strips.end(),[begin,end](ChannelStrip& strip)
		{strip.colorPresets(begin,end);});
	return *this;
	}

static void gain_update(float g,Slider& s,TextEntry& e)
	{
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
	s.value(gain_map_inv(g));
	}


void MixerConsole::changed(Slider& slider,SliderId id)
	{
	switch(id)
		{
		case SliderId::MASTER_GAIN:
			m_master_gain=gain_map(slider.value());
		//	TODO: forward message to owner
			gain_update(m_master_gain,slider,m_master_entry);
			break;
		}
	}


void MixerConsole::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::MASTER_GAIN:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{
				m_master_gain=val_new;
			//	TODO: forward message to owner
				}
			gain_update(m_master_gain,m_master_slider,entry);
			}
			break;
		}
	}

MixerConsole::MixerConsole(Container& cnt,Session& session):
	 m_master_gain(session.gainGet())
	,m_sections(cnt,false)
		,m_channels(m_sections.insertMode({0,Box::EXPAND|Box::FILL}))
			,m_strip_box(m_channels,false)
				,m_strips(session.channelsCountGet()
					,[this,&session](ChannelStrip* mem,int k)
						{
						new(mem)ChannelStrip(m_strip_box.insertMode({0,Box::EXPAND|Box::FILL}),session.channelViewGet(k));
						m_separators.append(Separator(m_strip_box.insertMode({2,Box::EXPAND|Box::FILL}),true));
						})
		,m_master(m_sections.insertMode({2,0}),true)
			,m_master_label(m_master,"Master out")
			,m_master_slider(m_master.insertMode({0,Box::EXPAND|Box::FILL}),true)
			,m_master_entry(m_master.insertMode({0,0}))
	{
	m_channels.directions(ScrolledWindow::HORIZONTAL);
	m_master_entry.small(1).width(7);

	m_master_slider.callback(*this,SliderId::MASTER_GAIN);
	m_master_entry.callback(*this,TextEntryId::MASTER_GAIN);

	m_master_slider.value(gain_map_inv(m_master_gain));
	}
