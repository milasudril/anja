//@	{
//@	 "targets":[{"name":"sessioneditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessioneditor.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONEDITOR_HPP
#define ANJA_SESSIONEDITOR_HPP

#include "tabview.hpp"
#include "waveformeditor.hpp"
#include "mixerconsole.hpp"

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

			void masterGainChanged(MixerConsole& mixer,MixerId id);

		private:
			Session& r_session;
			TabView m_tabs;
				WaveformEditor m_waveform;
				MixerConsole m_mixer;
		//		SettingsPanel m_settings;
		};
	}

#endif // ANJA_SESSIONEDITOR_HPP
