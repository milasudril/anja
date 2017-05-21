//@	{
//@	 "targets":[{"name":"channelview.o","type":"object"}]
//@	}

#include "channelview.hpp"

using namespace Anja;

void ChannelView::load(const SessionFileRecord& rec)
	{
	*r_channel=Channel(rec);
	*r_channel_data=ChannelData(rec);
	}

void ChannelView::store(SessionFileRecord& rec)
	{
	r_channel->dataGet(rec);
	r_channel_data->dataGet(rec);
	}
