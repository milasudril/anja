//@	{"targets":[{"name":"mixerconsole.o","type":"object"}]}

#include "mixerconsole.hpp"
#include "../sessiondata/session.hpp"

using namespace Anja;


MixerConsole::MixerConsole(Container& cnt,Session& session):
	m_strip_box(cnt,false)
		,m_strips(session.channelsCountGet()
			,[this,&session](ChannelStrip* mem,int k)
				{
				new(mem)ChannelStrip(m_strip_box.insertMode({0,Box::EXPAND|Box::FILL}),session.channelViewGet(k));
				m_separators.append(Separator(m_strip_box.insertMode({2,Box::EXPAND|Box::FILL}),true));
				})
		,m_master(m_strip_box.insertMode({2,0}),true)
			,m_master_label(m_master,"Master out")
			,m_master_slider(m_master.insertMode({0,Box::EXPAND|Box::FILL}),true)
			,m_master_entry(m_master.insertMode({0,0}))
	{
	m_master_entry.small(1).width(7);
	}
