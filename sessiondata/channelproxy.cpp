//@	{
//@	 "targets":[{"name":"channelproxy.o","type":"object"}]
//@	}

#include "channelproxy.hpp"

using namespace Anja;

ChannelProxy& ChannelProxy::load(const SessionFileRecord& rec)
	{
	*r_channel=Channel(rec);
	*r_channel_data=ChannelData(rec);
	return *this;
	}

const ChannelProxy& ChannelProxy::store(SessionFileRecord& rec) const
	{
	r_channel->store(rec);
	r_channel_data->store(rec);
	return *this;
	}
