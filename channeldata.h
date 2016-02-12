#ifdef __WAND__
target[name[channeldata.h] type[include]]
dependency[channeldata.o]
#endif

#ifndef CHANNELDATA_H
#define CHANNELDATA_H

#include "channel.h"

#include "framework/keyboardlayout.h"
#include "framework/arraydynamicshort.h"

class SessionFileRecord;

class Channel;

class ChannelData
	{
	public:
		ChannelData();
		ChannelData(const SessionFileRecord& record,Channel& channel
			,KeyboardLayout::KeyDescriptor* key);

		void init(Channel& ch,KeyboardLayout::KeyDescriptor* key,unsigned int k);

		const ArrayDynamicShort<char>& labelGet() const noexcept
			{return m_label;}

		ChannelData& labelSet(const char* label) noexcept;

		ChannelData& channelSet(Channel& channel) noexcept
			{
			r_channel=&channel;
			return *this;
			}

		const Channel& channelGet() const noexcept
			{return *r_channel;}

		Channel& channelGet() noexcept
			{return *r_channel;}

		void keySet(KeyboardLayout::KeyDescriptor* key) noexcept
			{r_key=key;}

		const ColorRGBA& colorGet() const noexcept
			{return m_color;}

		void colorSet(const ColorRGBA& color) noexcept
			{
			m_color=color;
			if(r_key!=nullptr)
				{r_key->colorBackgroundSet(color);}
			m_state_flags|=DIRTY;
			}

		void dataGet(SessionFileRecord& record) const;

		bool dirtyIs() const noexcept
			{return m_state_flags&DIRTY || r_channel->dirtyIs();}

		void dirtyClear() noexcept
			{
			m_state_flags&=~DIRTY;
			r_channel->dirtyClear();
			}

	private:
		ArrayDynamicShort<char> m_label;
		ColorRGBA m_color;
		Channel* r_channel;
		KeyboardLayout::KeyDescriptor* r_key;

		static constexpr unsigned int DIRTY=0x1;

		unsigned int m_state_flags;
	};

#endif
