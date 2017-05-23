//@	 {"targets":[{"name":"sessioneditor.o","type":"object"}]}

#include "sessioneditor.hpp"
#include "../sessiondata/session.hpp"

using namespace Anja;

static constexpr uint8_t s_channel_scancodes[]
	{
	 59,60,61,62
	,63,64,65,66
	,67,68,87,88
	};

static constexpr uint8_t scancode_channels(uint8_t scancode)
	{
	if(scancode>=59 && scancode<69)
		{return scancode - 59;}
	if(scancode>=87 && scancode<89)
		{return 10 + (scancode - 87);}
	return 0xff;
	}

constexpr uint8_t s_slot_scancodes[]=
	{
	 41,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  12, 13, 14
	,16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28
	,30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43
	,86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53
	};

static constexpr ArrayFixed<uint8_t,Wavetable::length()> gen_scancode_slots()
	{
	ArrayFixed<uint8_t,Wavetable::length()> ret;
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


void SessionEditor::nameChanged(ChannelStrip& strip,int id)
	{
	m_waveform.channelName(id,strip.name().begin());
	if(id>=0 && id<12)
		{m_keyboard.keyLabel(s_channel_scancodes[id],strip.name().begin()).redraw();}
	}

void SessionEditor::colorChanged(ChannelStrip& strip,int id)
	{
	if(id>=0 && id<12)
		{
		m_keyboard.keyColor(s_channel_scancodes[id],strip.color()).redraw();
		}
	}


void SessionEditor::descriptionChanged(WaveformEditor& wf,WaveformEditId id)
	{
	auto slot=r_session.slotActiveGet();
	if(slot>=0 && slot<sizeof(s_slot_scancodes));
		{
		m_keyboard.keyLabel(s_slot_scancodes[slot]
			,r_session.waveformViewGet(slot).keyLabelGet().begin()).redraw();
		}
	}

void SessionEditor::colorChanged(WaveformEditor& wf,WaveformEditId id)
	{
	auto slot=r_session.slotActiveGet();
	if(slot>=0 && slot<sizeof(s_slot_scancodes));
		{
		m_keyboard.keyColor(s_slot_scancodes[slot]
			,r_session.waveformViewGet(slot).keyColorGet()).redraw();
		}
	}

void SessionEditor::colorPresetsChanged(ColorPicker& picker)
	{
	r_session.colorPresetsSet(picker.presetsBegin(),picker.presetsEnd());
	m_waveform.colorPresets(r_session.colorPresetsGet());
	m_mixer.colorPresets(r_session.colorPresetsGet());
	}

void SessionEditor::masterGainChanged(MixerConsole& mixer,MixerId id)
	{
	r_session.gainSet(mixer.masterGain());
	}

void SessionEditor::indexSelected(KeyboardView& keyboard,KeyboardViewId id)
	{
	auto scancode=keyboard.selection();
	auto index=scancode_channels(scancode);
	if(index!=0xff)
		{
		m_tabs.activate(1);
		m_mixer.focus(index);
		return;
		}

	index=s_scancode_slots[scancode];
	if(index!=0xff)
		{
		m_tabs.activate(0);
		m_waveform.waveform(r_session.waveformViewGet(index));
		r_session.slotActiveSet(index);
		}

	auto slot=r_session.slotActiveGet();
	if(slot>=0 && slot<sizeof(s_slot_scancodes));
		{keyboard.selection(s_slot_scancodes[slot]);}
	}

SessionEditor& SessionEditor::sessionUpdated()
	{
	m_keyboard.reset();
		{
		auto N=std::min(r_session.channelsCountGet(),12);
		for(decltype(N) k=0;k<N;++k)
			{
			auto key=s_channel_scancodes[k];
			auto ch=r_session.channelViewGet(k);
			m_keyboard.keyColor(key,ch.colorGet()).keyLabel(key,ch.labelGet().begin());
			}
		}

		{
		auto N=std::min(static_cast<int>(sizeof(s_slot_scancodes)),r_session.slotsCountGet());
		for(decltype(N) k=0;k<N;++k)
			{
			auto key=s_slot_scancodes[k];
			auto wf=r_session.waveformViewGet(k);
			m_keyboard.keyColor(key,wf.keyColorGet());
			if(wf.keyLabelGet().length()!=0)
				{m_keyboard.keyLabel(key,wf.keyLabelGet().begin());}
			}
		}

		{
		auto slot=r_session.slotActiveGet();
		if(slot>=0 && slot<sizeof(s_slot_scancodes));
			{m_keyboard.selection(s_slot_scancodes[slot]);}
		}
	m_keyboard.redraw();

	auto& color_presets=r_session.colorPresetsGet();
	auto channel_names=r_session.channelLabelsGet();

	m_waveform.colorPresets(color_presets)
		.channelNames(channel_names)
		.waveform(r_session.waveformViewGet(r_session.slotActiveGet()))
		.waveformUpdate();

	m_mixer.colorPresets(color_presets).channels(r_session);

	m_settings.sessionUpdated();
	return *this;
	}


SessionEditor::SessionEditor(Container& cnt,Session& session)
	:r_session(session)
	,m_hsplit(cnt,true)
		,m_keyboard(m_hsplit.insertMode({Anja::Paned::SHRINK_ALLOWED|Anja::Paned::RESIZE}))
		,m_tabs(m_hsplit)
			,m_waveform(m_tabs.tabTitle("Waveform")
				,session.waveformViewGet(session.slotActiveGet())
				,session.channelLabelsGet())
			,m_mixer(m_tabs.tabTitle("Channel Mixer"),session)
			,m_settings(m_tabs.tabTitle("Session"),session)
	{

	sessionUpdated();

	m_keyboard.callback(*this,KeyboardViewId::KEYBOARD_MAIN);
	m_waveform.colorPresets(session.colorPresetsGet())
		.callback(*this,WaveformEditId::WAVEFORM_CURRENT);
	m_mixer.colorPresets(session.colorPresetsGet());
	m_mixer.channelsCallback(*this)
		.callback(*this,MixerId::CHANNEL_MIXER);
	}
