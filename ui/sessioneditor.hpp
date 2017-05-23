//@	{
//@	 "targets":[{"name":"sessioneditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessioneditor.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONEDITOR_HPP
#define ANJA_SESSIONEDITOR_HPP

#include "tabview.hpp"
#include "waveformeditor.hpp"
#include "mixerconsole.hpp"
#include "paned.hpp"
#include "keyboardview.hpp"
#include "sessionproperties.hpp"

namespace Anja
	{
	class Session;
	class SessionEditor
		{
		public:
			SessionEditor& operator=(SessionEditor&&)=delete;
			SessionEditor(SessionEditor&&)=delete;

			SessionEditor(Container& cnt,Session& session);

			void channelNameChanged(ChannelStrip& strip,int id);
			void channelColorChanged(ChannelStrip& strip,int id);
			void colorPresetsChanged(ColorPicker& picker);


			enum class MixerId:int{CHANNEL_MIXER};
			enum class WaveformEditId:int{WAVEFORM_CURRENT};
			enum class KeyboardViewId:int{KEYBOARD_MAIN};

			void indexSelected(KeyboardView& keyboard,KeyboardViewId id);
			void waveformDescriptionChanged(WaveformEditor& wf,WaveformEditId id);
			void waveformColorChanged(WaveformEditor& wf,WaveformEditId id);
			void masterGainChanged(MixerConsole& mixer,MixerId id);

			SessionEditor& sessionUpdated();

		private:
			Session& r_session;
			Paned m_hsplit;
				KeyboardView m_keyboard;
				TabView m_tabs;
					WaveformEditor m_waveform;
					MixerConsole m_mixer;
					SessionProperties m_settings;
		};
	}

#endif // ANJA_SESSIONEDITOR_HPP
