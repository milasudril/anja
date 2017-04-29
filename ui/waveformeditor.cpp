//@	{"targets":[{"name":"waveformeditor.o","type":"object"}]}

#include "waveformeditor.hpp"
#include "../common/colorstring.hpp"

using namespace Anja;

enum{FILENAME,DESCRIPTION,COLOR};

WaveformEditor::WaveformEditor(Container& cnt,const WaveformView& waveform):
	 m_waveform(waveform)
	,m_box(cnt,true)
		,m_filename(m_box.insertMode({1,0}),false)
			,m_filename_label(m_filename.insertMode({2,0}),"Source")
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}),FILENAME)
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}),DESCRIPTION)
		,m_details(m_box,false)
			,m_details_left(m_details.insertMode(Paned::InsertMode{Paned::SHRINK_ALLOWED|Paned::RESIZE|Paned::SCROLL_ALLOWED}),true)
				,m_color(m_details_left.insertMode({2,0}),false)
					,m_color_label(m_color.insertMode({2,0}),"Color")
					,m_color_input(m_color,COLOR)
	{
	m_filename_input.content(waveform.filenameGet().begin());
	m_description_input.content(waveform.descriptionGet().begin());
	m_color_input.content(ColorString(waveform.keyColorGet()).begin());
	}
