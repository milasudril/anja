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
	if(scancode>=87 && scancode<88)
		{return 10 + (scancode - 87);}
	return 0xff;
	}

void SessionEditor::channelNameChanged(ChannelStrip& strip,int id)
	{
	m_waveform.channelName(id,strip.name().begin());
//	m_keyboard.channelName(id,strip.name().begin());
	}

void SessionEditor::channelColorChanged(ChannelStrip& strip,int id)
	{
	if(id>=0 && id<12)
		{m_keyboard.keyColor(s_channel_scancodes[id],strip.color());}
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

SessionEditor::SessionEditor(Container& cnt,Session& session)
	:r_session(session)
	,m_hsplit(cnt,true)
		,m_keyboard(m_hsplit.insertMode({Anja::Paned::SHRINK_ALLOWED|Anja::Paned::RESIZE}))
		,m_tabs(m_hsplit)
			,m_waveform(m_tabs.tabTitle("Waveform")
				,session.waveformViewGet(session.slotActiveGet())
				,session.channelLabelsGet())
			,m_mixer(m_tabs.tabTitle("Channel Mixer"),session)
	{

		{
		auto N=std::min(session.channelsCountGet(),12);
		for(decltype(N) k=0;k<N;++k)
			{
			m_keyboard.keyColor(s_channel_scancodes[k],session.channelViewGet(k).colorGet());
			}
		}



	m_waveform.colorPresets(session.colorPresetsGet());
	m_mixer.colorPresets(session.colorPresetsGet());
	m_mixer.channelsCallback(*this)
		.callback(*this,MixerId::CHANNEL_MIXER);
	}
