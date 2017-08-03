//@	 {"targets":[{"name":"sessioneditor.o","type":"object"}]}

#include "sessioneditor.hpp"
#include "../sessiondata/session.hpp"
#include "../sessiondata/keymap.hpp"

using namespace Anja;

void SessionEditor::nameChanged(ChannelStrip& strip,int id)
	{
	m_waveform.channelName(id,strip.name().begin());
	if(id>=0 && id<12)
		{m_keyboard.keyLabel(channelToScancode(id),strip.name().begin()).redraw();}
	if(r_cb_obj!=nullptr)
		{m_channel_name_callback(r_cb_obj,strip,id);}
	}

void SessionEditor::colorChanged(ChannelStrip& strip,int id)
	{
	if(id>=0 && id<12)
		{
		m_keyboard.keyColor(channelToScancode(id),strip.color()).redraw();
		}
	}


void SessionEditor::gainChanged(ChannelStrip& strip,int id)
	{
	if(r_cb_obj!=nullptr)
		{m_channel_gain_callback(r_cb_obj,strip,id);}
	}


void SessionEditor::descriptionChanged(WaveformEditor& wf,WaveformEditId id)
	{
	auto slot=r_session.slotActive();
	auto scancode=slotToScancode(slot);
	if(scancode!=0xff)
		{
		m_keyboard.keyLabel(scancode,r_session.waveformProxy(slot).keyLabel().begin())
			.keyLabel(Keys::AUDITION
				,Anja::String("Audition: ")
					.append(r_session.waveformProxy(slot)
					.keyLabel()).begin())
			.redraw();
		}
	}

void SessionEditor::entryChanged(WaveformEditor& wf,WaveformEditId id,int textbox_id)
	{
	if(r_cb_obj!=nullptr)
		{m_waveform_entry_changed(r_cb_obj,textbox_id);}
	}

void SessionEditor::waveformEntryChangeCommit(int textbox_id)
	{
	m_waveform.changeCommit(static_cast<WaveformEditor::TextEntryId>(textbox_id));
	}


void SessionEditor::colorChanged(WaveformEditor& wf,WaveformEditId id)
	{
	auto slot=r_session.slotActive();
	auto scancode=slotToScancode(slot);
	if(scancode!=0xff)
		{
		m_keyboard.keyColor(scancode,r_session.waveformProxy(slot).keyColor())
			.redraw();
		}
	}

void SessionEditor::colorPresetsChanged(ColorPicker& picker)
	{
	r_session.colorPresets(picker.presetsBegin(),picker.presetsEnd());
	m_waveform.colorPresets(r_session.colorPresets());
	m_mixer.colorPresets(r_session.colorPresets());
	}

void SessionEditor::masterGainChanged(MixerConsole& mixer,MixerId id)
	{
	r_session.gain(mixer.masterGain());
	}

void SessionEditor::indexSelected(KeyboardView& keyboard,KeyboardViewId id)
	{
	auto scancode=keyboard.selection();
	auto index=scancodeToChannel(scancode);
	if(index!=-1)
		{
		m_tabs.activate(1);
		return;
		}

	index=scancodeToSlot(scancode);
	if(index!=0xff)
		{
		m_tabs.activate(0);
		m_waveform.waveform(r_session.waveformProxy(index));
		r_session.slotActive(index);
		}

	auto slot=r_session.slotActive();
	scancode=slotToScancode(slot);
	if(scancode!=0xff)
		{
		keyboard.selection(scancode).keyLabel(Keys::AUDITION
			,Anja::String("Audition: ").append(r_session.waveformProxy(slot).keyLabel()).begin());
		}
	}

SessionEditor& SessionEditor::sessionUpdated()
	{
	m_keyboard.reset().keyLabel(Keys::AUDITION,"Audition");
		{
		auto N=std::min(r_session.channelsCount(),12);
		for(decltype(N) k=0;k<N;++k)
			{
			auto key=channelToScancode(k);
			auto ch=r_session.channelProxy(k);
			m_keyboard.keyColor(key,ch.color()).keyLabel(key,ch.label().begin());
			}
		}

		{
		auto N=r_session.slotsCountGet();
		for(decltype(N) k=0;k<N;++k)
			{
			auto scancode=slotToScancode(k);
			if(scancode==0xff)
				{break;}
			auto wf=r_session.waveformProxy(k);
			m_keyboard.keyColor(scancode,wf.keyColor());
			if(wf.keyLabel().length()!=0)
				{m_keyboard.keyLabel(scancode,wf.keyLabel().begin());}
			}
		}

		{
		auto slot=r_session.slotActive();
		auto scancode=slotToScancode(slot);
		if(scancode!=0xff)
			{
			m_keyboard.selection(scancode).keyLabel(Keys::AUDITION
				,Anja::String("Audition: ").append(r_session.waveformProxy(slot).keyLabel()).begin());
			}
		}
	m_keyboard.redraw();

	auto& color_presets=r_session.colorPresets();
	auto channel_names=r_session.channelLabels();

	m_waveform.colorPresets(color_presets)
		.channelNames(channel_names)
		.waveform(r_session.waveformProxy(r_session.slotActive()))
		.waveformUpdate();

	m_mixer.colorPresets(color_presets).channels(r_session);

	m_settings.sessionUpdated();
	return *this;
	}


SessionEditor::SessionEditor(Container& cnt,const ImageRepository& images,Session& session)
	:r_cb_obj(nullptr)
	,r_session(session)
	,m_hsplit(cnt,true)
		,m_keyboard(m_hsplit.insertMode({Anja::Paned::SHRINK_ALLOWED|Anja::Paned::RESIZE}))
		,m_tabs(m_hsplit)
			,m_waveform(m_tabs.tabTitle("Waveform"),images
				,session.waveformProxy(session.slotActive())
				,session.channelLabels())
			,m_mixer(m_tabs.tabTitle("Channel mixer"),session)
			,m_settings(m_tabs.tabTitle("Session"),session)
	{

	sessionUpdated();

	m_keyboard.callback(*this,KeyboardViewId::KEYBOARD_MAIN);
	m_waveform.colorPresets(session.colorPresets())
		.callback(*this,WaveformEditId::WAVEFORM_CURRENT);
	m_mixer.colorPresets(session.colorPresets());
	m_mixer.channelsCallback(*this)
		.callback(*this,MixerId::CHANNEL_MIXER);
	}
