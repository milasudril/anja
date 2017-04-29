//@	{
//@	 "targets":[{"name":"channelview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channelview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CHANNELVIEW_HPP
#define ANJA_CHANNELVIEW_HPP

#include "channel.hpp"
#include "channeldata.hpp"
#include "../common/keyboardlayout.hpp"

namespace Anja
	{
	class ChannelView
		{
		public:
			explicit ChannelView(Channel& ch,ChannelData& ch_data
				,KeyboardLayout::KeyDescriptor* key) noexcept:r_channel(ch)
				,r_channel_data(ch_data),r_key(key)
				{}

			void load(const SessionFileRecord& rec);
			void store(SessionFileRecord& rec);

			const String& labelGet() const noexcept
				{return r_channel_data.labelGet();}

			void labelSet(const String& label)
				{
				r_channel_data.labelSet(label);
				if(r_key!=nullptr)
					{r_key->labelSet(r_channel_data.labelGet().begin());}
				}

			const ColorRGBA& colorGet() const noexcept
				{return r_channel_data.colorGet();}

			void colorSet(const ColorRGBA& color) noexcept
				{
				r_channel_data.colorSet(color);
				if(r_key!=nullptr)
					{r_key->colorBackgroundSet(r_channel_data.colorGet());}
				}

			bool dirtyIs() const noexcept
				{return r_channel.dirtyIs() || r_channel_data.dirtyIs();}

			void dirtyClear() noexcept
				{
				r_channel.dirtyClear();
				r_channel_data.dirtyClear();
				}

			Channel& channel() noexcept
				{return r_channel;}

			const Channel& channel() const noexcept
				{return r_channel;}

		private:
			Channel& r_channel;
			ChannelData& r_channel_data;
			KeyboardLayout::KeyDescriptor* r_key;
		};
	}

#endif // ANJA_CHANNELVIEW_HPP
