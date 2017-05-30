//@	{"targets":[{"name":"keymap.o","type":"object"}]}


#include "keymap.hpp"
#include "../common/arrayfixed.hpp"

using namespace Anja;

static constexpr uint8_t s_channel_scancodes[]
	{
	 59,60,61,62
	,63,64,65,66
	,67,68,87,88
	};

uint8_t Anja::channelToScancode(int channel)
	{
	if(channel>=0 && channel<16)
		{return s_channel_scancodes[channel];}
	return 0xff;
	}

constexpr uint8_t s_slot_scancodes[]=
	{
	 41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  12, 13, 14
	,16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28
	,30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43
	,86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53
	};

uint8_t Anja::slotToScancode(uint8_t slot)
	{
	if(slot>=0 && slot<sizeof(s_slot_scancodes))
		{return s_slot_scancodes[slot];}
	return 0xff;
	}

static constexpr ArrayFixed<uint8_t,128> gen_scancode_slots()
	{
	ArrayFixed<uint8_t,128> ret;
	for(decltype(ret.length()) k=0;k<ret.length();++k)
		{
		uint8_t index=0xff;
		for(size_t l=0;l<sizeof(s_slot_scancodes);++l)
			{
			if(s_slot_scancodes[l]==k)
				{
				index=l;
				break;
				}
			}
		ret[k]=index;
		}
	return ret;
	}

static constexpr auto s_scancode_slots=gen_scancode_slots();

uint8_t Anja::scancodeToSlot(uint8_t slot)
	{
	if(slot>=0 && slot<s_scancode_slots.length())
		{return s_scancode_slots[slot];}
	return 0xff;
	}



template<int k>
static constexpr bool mappingCheck()
	{
	static_assert(midiToSlot(slotToMIDI(k))==k,"");
	static_assert(slotToMIDI(midiToSlot(k))==k,"");
	return mappingCheck<k+1>();
	}

template<>
constexpr bool mappingCheck<128>()
	{
	return true;
	}

static_assert(mappingCheck<0>(),"MIDI<=>Slot mapping is wrong");
