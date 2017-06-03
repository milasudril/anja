//@	{
//@	 "targets":[{"name":"channelproxy.o","type":"object"}]
//@	}

#include "channelproxy.hpp"

using namespace Anja;

void ChannelProxy::load(const SessionFileRecord& rec)
	{
	*r_channel=Channel(rec);
	*r_channel_data=ChannelData(rec);
	}

void ChannelProxy::store(SessionFileRecord& rec)
	{
	r_channel->dataGet(rec);
	r_channel_data->dataGet(rec);
	}
