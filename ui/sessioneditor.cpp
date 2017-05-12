//@	 {"targets":[{"name":"sessioneditor.o","type":"object"}]}

#include "sessioneditor.hpp"
#include "../sessiondata/session.hpp"

using namespace Anja;

void SessionEditor::channelNameChanged(ChannelStrip& strip,int id)
	{
	m_waveform.channelName(id,strip.name().begin());
//	m_keyboard.channelName(id,strip.name().begin());
	}

void SessionEditor::channelColorChanged(ChannelStrip& strip,int id)
	{
//	Keyboard view....
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
	,m_tabs(cnt)
		,m_waveform(m_tabs.tabTitle("Waveform")
			,session.waveformViewGet(session.slotActiveGet())
			,session.channelLabelsGet())
		,m_mixer(m_tabs.tabTitle("Channel Mixer"),session)
	{
	m_waveform.colorPresets(session.colorPresetsGet());
	m_mixer.colorPresets(session.colorPresetsGet());
	m_mixer.channelsCallback(*this)
		.callback(*this,MixerId::CHANNEL_MIXER);
	}
