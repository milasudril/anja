//@	{"targets":[{"name":"waveformeditor.o","type":"object"}]}

#include "waveformeditor.hpp"
#include "../sessiondata/session.hpp"
#include "../common/colorstring.hpp"
#include "../common/arraysimple.hpp"

using namespace Anja;

enum{FILENAME,FILENAME_BROWSE,DESCRIPTION,COLOR,COLOR_PICK,CHANNEL,OPTIONS};

WaveformEditor::WaveformEditor(Container& cnt,const WaveformView& waveform
	,const ArraySimple<String>& channel_names):
	 m_waveform(waveform)
	,m_box(cnt,true)
		,m_filename(m_box.insertMode({1,0}),false)
			,m_filename_label(m_filename.insertMode({2,0}),"Source")
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}),FILENAME)
			,m_filename_browse(m_filename.insertMode({2,0}),FILENAME_BROWSE,"Browse")
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}),DESCRIPTION)
		,m_details(m_box.insertMode({2,Box::EXPAND|Box::FILL}),false)
			,m_details_left(m_details.insertMode({Paned::SHRINK_ALLOWED|Paned::RESIZE|Paned::SCROLL_ALLOWED}),true)
				,m_color(m_details_left.insertMode({2,0}),false)
					,m_color_label(m_color.insertMode({2,0}),"Color")
					,m_color_input(m_color.insertMode({2,Box::EXPAND|Box::FILL}),COLOR)
					,m_color_pick(m_color.insertMode({2,0}),COLOR_PICK,"…")
				,m_channel(m_details_left,false)
					,m_channel_label(m_channel.insertMode({2,0}),"Channel")
					,m_channel_input(m_channel.insertMode({2,Box::EXPAND|Box::FILL}),CHANNEL)
				,m_options(m_details_left,false)
					,m_options_label(m_options.insertMode({2,0}),"Options")
				,m_options_input(m_details_left.insertMode({0,Box::EXPAND|Box::FILL}),OPTIONS,true)
	{
	m_filename_browse.callback(*this);
	m_color_pick.callback(*this);
	m_options_input.callback(*this);

	m_filename_input.content(waveform.filenameGet().begin());
	m_description_input.content(waveform.descriptionGet().begin());

	m_color_input.content(ColorString(waveform.keyColorGet()).begin());

	std::for_each(channel_names.begin(),channel_names.end(),[this](const String& str)
		{m_channel_input.append(str.begin());});

	m_channel_input.selected(m_waveform.channelGet());
	m_options_input.append(waveform.flagNames());
	m_options_input.selected(waveform.flagsGet());
	}

void WaveformEditor::clicked(Button& src)
	{
	src.state(0);
	}

void WaveformEditor::clicked(OptionList<WaveformEditor>& src,Checkbox& opt)
	{
	}