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
#include "sessionpropertieseditor.hpp"

namespace Anja
	{
	class Session;
	class SessionEditor
		{
		public:
			SessionEditor& operator=(SessionEditor&&)=delete;
			SessionEditor(SessionEditor&&)=delete;

			SessionEditor(Container& cnt,Session& session);

			enum class MixerId:int{CHANNEL_MIXER};
			enum class WaveformEditId:int{WAVEFORM_CURRENT};
			enum class KeyboardViewId:int{KEYBOARD_MAIN};

			void nameChanged(ChannelStrip& strip,int id);
			void colorChanged(ChannelStrip& strip,int id);
			void colorPresetsChanged(ColorPicker& picker);

			void masterGainChanged(MixerConsole& mixer,MixerId id);

			void indexSelected(KeyboardView& keyboard,KeyboardViewId id);
			void descriptionChanged(WaveformEditor& wf,WaveformEditId id);
			void colorChanged(WaveformEditor& wf,WaveformEditId id);


			SessionEditor& sessionUpdated();

			template<class Callback,class IdType>
			SessionEditor& sessionChangedCallback(Callback& cb,IdType id)
				{
				m_settings.callback(cb,id);
				return *this;
				}

		private:
			Session& r_session;
			Paned m_hsplit;
				KeyboardView m_keyboard;
				TabView m_tabs;
					WaveformEditor m_waveform;
					MixerConsole m_mixer;
					SessionPropertiesEditor m_settings;
		};
	}

#endif // ANJA_SESSIONEDITOR_HPP
