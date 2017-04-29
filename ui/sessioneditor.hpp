//@	{
//@	 "targets":[{"name":"sessioneditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sessioneditor.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSIONEDITOR_HPP
#define ANJA_SESSIONEDITOR_HPP

#include "tabview.hpp"
#include "waveformeditor.hpp"

namespace Anja
	{
	class Session;
	class SessionEditorBase
		{
		public:
			SessionEditorBase(Container& cnt,Session& session);
		private:
			Session& r_session;
			TabView m_tabs;
				WaveformEditor m_waveform;
		//		Mixer m_mixer;
		//		SettingsPanel m_settings;
		};
	}

#endif // ANJA_SESSIONEDITOR_HPP
