//@	{
//@	 "targets":[{"name":"channelproxy.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channelproxy.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CHANNELPROXY_HPP
#define ANJA_CHANNELPROXY_HPP

#include "channel.hpp"
#include "channeldata.hpp"

namespace Anja
	{
	class ChannelProxy
		{
		public:
			explicit ChannelProxy(Channel& ch,ChannelData& ch_data) noexcept:r_channel(&ch)
				,r_channel_data(&ch_data)
				{}

			void load(const SessionFileRecord& rec);
			void store(SessionFileRecord& rec);

			const String& labelGet() const noexcept
				{return r_channel_data->labelGet();}

			void labelSet(String&& label)
				{r_channel_data->labelSet(std::move(label));}

			void labelSet(const char* label)
				{r_channel_data->labelSet(label);}

			const ColorRGBA& colorGet() const noexcept
				{return r_channel_data->colorGet();}

			void colorSet(const ColorRGBA& color) noexcept
				{r_channel_data->colorSet(color);}



			float gainGet() const noexcept
				{return r_channel->gainGet();}

			void gainSet(float gain) noexcept
				{r_channel->gainSet(gain);}

			float fadeTimeGet() const noexcept
				{return r_channel->fadeTimeGet();}

			void fadeTimeSet(float time) noexcept
				{return r_channel->fadeTimeSet(time);}



			bool dirtyIs() const noexcept
				{return r_channel->dirtyIs() || r_channel_data->dirtyIs();}

			void dirtyClear() noexcept
				{
				r_channel->dirtyClear();
				r_channel_data->dirtyClear();
				}

		private:
			Channel* r_channel;
			ChannelData* r_channel_data;
		};
	}

#endif // ANJA_CHANNELVIEW_HPP