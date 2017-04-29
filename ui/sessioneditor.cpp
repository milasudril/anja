//@	 {"targets":[{"name":"sessioneditor.o","type":"object"}]}

#include "sessioneditor.hpp"
#include "../sessiondata/session.hpp"

using namespace Anja;

SessionEditorBase::SessionEditorBase(Container& cnt,Session& session)
	:r_session(session)
	,m_tabs(cnt)
		,m_waveform(m_tabs.tabTitle("Waveform"),session.waveformViewGet(0))
	{}
