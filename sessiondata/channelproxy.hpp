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

			ChannelProxy& load(const SessionFileRecord& rec);
			const ChannelProxy& store(SessionFileRecord& rec) const;

			const String& label() const noexcept
				{return r_channel_data->label();}

			ChannelProxy& label(String&& label)
				{
				r_channel_data->label(std::move(label));
				return *this;
				}

			ChannelProxy& label(const char* label)
				{
				r_channel_data->label(label);
				return *this;
				}

			const ColorRGBA& color() const noexcept
				{return r_channel_data->color();}

			ChannelProxy& color(const ColorRGBA& color) noexcept
				{
				r_channel_data->color(color);
				return *this;
				}



			float gain() const noexcept
				{return r_channel->gain();}

			ChannelProxy& gain(float gain) noexcept
				{
				r_channel->gain(gain);
				return *this;
				}

			float fadeTime() const noexcept
				{return r_channel->fadeTime();}

			ChannelProxy& fadeTime(float time) noexcept
				{
				r_channel->fadeTime(time);
				return *this;
				}



			bool dirty() const noexcept
				{return r_channel->dirty() || r_channel_data->dirty();}

			ChannelProxy& dirtyClear() noexcept
				{
				r_channel->dirtyClear();
				r_channel_data->dirtyClear();
				return *this;
				}

		private:
			Channel* r_channel;
			ChannelData* r_channel_data;
		};
	}

#endif // ANJA_CHANNELVIEW_HPP
