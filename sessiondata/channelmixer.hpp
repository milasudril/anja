//@	{"targets":[{"name":"channelmixer.hpp","type":"include"}]}

#ifndef ANJA_CHANNELMIXER_HPP
#define ANJA_CHANNELMIXER_HPP

#include "channel.hpp"
#include "../common/arrayfixed.hpp"

namespace Anja
	{
	class ChannelMixer:public ArrayFixed<Channel,16>
		{};
	}

#endif
