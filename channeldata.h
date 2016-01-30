#ifdef __WAND__
target[name[channeldata.h] type[include]]
dependency[channeldata.o]
#endif

#ifndef CHANNELDATA_H
#define CHANNELDATA_H

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

		const ArrayDynamicShort<char>& labelGet() const
			{return m_label;}

		ChannelData& labelSet(const char* label)
			{
			m_label=label;
			if(r_key!=nullptr)
				{r_key->labelSet(label);}
			return *this;
			}

		ChannelData& channelSet(Channel& channel)
			{
			r_channel=&channel;
			return *this;
			}

		const Channel& channelGet() const
			{return *r_channel;}

		Channel& channelGet()
			{return *r_channel;}

		void keySet(KeyboardLayout::KeyDescriptor* key)
			{r_key=key;}

	private:
		ArrayDynamicShort<char> m_label;
		Channel* r_channel;
		KeyboardLayout::KeyDescriptor* r_key;
	};

#endif
