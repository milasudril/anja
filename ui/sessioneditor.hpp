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
			SessionEditor(Container& cnt,ImageRepository&& images,Session& session)=delete;

			SessionEditor(Container& cnt,const ImageRepository& images,Session& session);

			enum class MixerId:int{CHANNEL_MIXER};
			enum class WaveformEditId:int{WAVEFORM_CURRENT};
			enum class KeyboardViewId:int{KEYBOARD_MAIN};

			void nameChanged(ChannelStrip& strip,int id);
			void colorChanged(ChannelStrip& strip,int id);
			void gainChanged(ChannelStrip& strip,int id);
			void colorPresetsChanged(ColorPicker& picker);

			void masterGainChanged(MixerConsole& mixer,MixerId id);

			void indexSelected(KeyboardView& keyboard,KeyboardViewId id);
			void descriptionChanged(WaveformEditor& wf,WaveformEditId id);
			void colorChanged(WaveformEditor& wf,WaveformEditId id);
			void entryChanged(WaveformEditor& wf,WaveformEditId id,int textbox_id);

			SessionEditor& sessionUpdated();

			SessionEditor& waveformShow()
				{
				m_tabs.activate(0);
				return *this;
				}

			SessionEditor& mixerShow()
				{
				m_tabs.activate(1);
				return *this;
				}

			SessionEditor& sessionShow()
				{
				m_tabs.activate(2);
				return *this;
				}

			template<class Callback,class IdType>
			SessionEditor& callback(Callback& cb,IdType id)
				{
				m_settings.callback(cb,id);
				m_channel_name_callback=[](void* cb_obj,ChannelStrip& self,int id)
					{reinterpret_cast<Callback*>(cb_obj)->nameChanged(self,id);};
				m_channel_gain_callback=[](void* cb_obj,ChannelStrip& self,int id)
					{reinterpret_cast<Callback*>(cb_obj)->gainChanged(self,id);};

				r_cb_obj=&cb;
				return *this;
				}

			Rectangle boundingBoxKeyboard() const noexcept
				{return m_hsplit.boundingBox(0);}

			Rectangle boundingBoxTabs() const noexcept
				{return m_hsplit.boundingBox(1);}

			Rectangle boundingBoxTrim() const noexcept
				{return m_waveform.boundingBoxTrim();}

			SessionEditor& waveformAutotrim() noexcept
				{
				m_waveform.autotrim();
				return *this;
				}

		private:
			void (*m_channel_name_callback)(void* cb_obj,ChannelStrip& self,int id);
			void (*m_channel_gain_callback)(void* cb_obj,ChannelStrip& self,int id);

			void* r_cb_obj;
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
