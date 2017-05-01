//@	{"targets":[{"name":"waveformeditor.o","type":"object"}]}

#include "waveformeditor.hpp"
#include "../sessiondata/session.hpp"
#include "../common/colorstring.hpp"
#include "../common/arraysimple.hpp"
#include "../common/floatconv.hpp"

#include <cstdio>

using namespace Anja;

enum
	{
	 FILENAME
	,FILENAME_BROWSE
	,FILENAME_RELOAD
	,DESCRIPTION
	,COLOR
	,COLOR_PICK
	,CHANNEL
	,GAIN_TEXT
	,GAIN_SLIDER
	,GAIN_RANDOM_TEXT
	,GAIN_RANDOM_SLIDER
	,OPTIONS
	,WAVEFORM
	,CURSORS_SWAP
	};

static double gain_map(double x)
	{return 6.0*x + (1-x)*(-72.0);}

static double gain_map_inv(double x)
	{return (x + 72)/(72 + 6);}

static double gain_random_map(double x)
	{return 12.0*x;}

static double gain_random_map_inv(double x)
	{return x/12.0;}

WaveformEditor::WaveformEditor(Container& cnt,const WaveformView& waveform
	,const ArraySimple<String>& channel_names):
	 m_waveform(waveform)
	,m_box(cnt,true)
		,m_filename(m_box.insertMode({1,0}),false)
			,m_filename_label(m_filename.insertMode({2,0}),"Source:")
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}),FILENAME)
			,m_filename_browse(m_filename.insertMode({2,0}),FILENAME_BROWSE,"Browse")
			,m_filename_reload(m_filename,FILENAME_RELOAD,"↺")
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description:")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}),DESCRIPTION)
		,m_details(m_box.insertMode({2,Box::EXPAND|Box::FILL}),false)
			,m_details_left(m_details.insertMode({Paned::SHRINK_ALLOWED|Paned::RESIZE|Paned::SCROLL_ALLOWED}),true)
				,m_color(m_details_left.insertMode({2,0}),false)
					,m_color_label(m_color.insertMode({2,0}),"Color:")
					,m_color_input(m_color.insertMode({2,Box::EXPAND|Box::FILL}),COLOR)
					,m_color_pick(m_color.insertMode({2,0}),COLOR_PICK,"…")
				,m_channel(m_details_left,false)
					,m_channel_label(m_channel.insertMode({2,0}),"Channel:")
					,m_channel_input(m_channel.insertMode({2,Box::EXPAND|Box::FILL}),CHANNEL)
				,m_gain(m_details_left,false)
					,m_gain_label(m_gain.insertMode({2,0}),"Gain/dBFS:")
					,m_gain_input(m_gain.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_gain_input_text(m_gain_input.insertMode({0,0}),GAIN_TEXT)
						,m_gain_input_slider(m_gain_input.insertMode({0,Box::EXPAND|Box::FILL}),GAIN_SLIDER,false)
				,m_gain_random(m_details_left,false)
					,m_gain_random_label(m_gain_random.insertMode({2,0}),"Gain random/dBFS:")
					,m_gain_random_input(m_gain_random.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_gain_random_input_text(m_gain_random_input.insertMode({0,0}),GAIN_RANDOM_TEXT)
						,m_gain_random_input_slider(m_gain_random_input.insertMode({0,Box::EXPAND|Box::FILL}),GAIN_RANDOM_SLIDER,false)
				,m_options(m_details_left,false)
					,m_options_label(m_options.insertMode({2,0}),"Options:")
				,m_options_input(m_details_left.insertMode({0,Box::EXPAND|Box::FILL}),OPTIONS,true)
			,m_details_right(m_details,true)
				,m_trim_panel(m_details_right.insertMode({0,0}),false)
					,m_swap(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),CURSORS_SWAP,"⇌")
	{
	m_gain_input_text.width(7).small(true).alignment(1);
	m_gain_random_input_text.width(6).small(true).alignment(1);

	m_filename_input.callback(*this);
	m_filename_browse.callback(*this);
	m_filename_reload.callback(*this);
	m_color_input.callback(*this);
	m_color_pick.callback(*this);
	m_channel_input.callback(*this);
	m_gain_input_slider.callback(*this);
	m_gain_input_text.callback(*this);
	m_gain_random_input_slider.callback(*this);
	m_gain_random_input_text.callback(*this);
	m_options_input.callback(*this);
//	TODO: Add m_waveform here...
	m_swap.callback(*this);

	std::for_each(channel_names.begin(),channel_names.end(),[this](const String& str)
		{m_channel_input.append(str.begin());});


	m_filename_input.content(waveform.filenameGet().begin());
	m_description_input.content(waveform.descriptionGet().begin());
	m_color_input.content(ColorString(waveform.keyColorGet()).begin());
	m_gain_input_slider.value(gain_map_inv(waveform.gainGet()));
	changed(m_gain_input_slider);
	m_gain_random_input_slider.value(gain_random_map_inv(waveform.gainRandomGet()));
	changed(m_gain_random_input_slider);

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
	switch(src.id())
		{
		case OPTIONS:
			if(opt.state())
				{m_waveform.flagSet(opt.id());}
			else
				{m_waveform.flagsUnset(opt.id());}
			break;
		}
	}

void WaveformEditor::changed(Slider& slider)
	{
	switch(slider.id())
		{
		case GAIN_SLIDER:
			{
			m_waveform.gainSet( gain_map(slider.value()) );
			char buffer[16];
			sprintf(buffer,"%.3f",m_waveform.gainGet());
			m_gain_input_text.content(buffer);
			}
			break;
		case GAIN_RANDOM_SLIDER:
			{
			m_waveform.gainRandomSet( gain_random_map(slider.value()) );
			char buffer[16];
			sprintf(buffer,"%.3f",m_waveform.gainRandomGet());
			m_gain_random_input_text.content(buffer);
			}
			break;
		}
	}

void WaveformEditor::changed(TextEntry& entry)
	{
	switch(entry.id())
		{
		case FILENAME:
		//	TODO: Pass message. We need to know session directory before trying
		//	to load a new file. After the filename has been successfully loaded,
		//	show its canonical path.
			entry.content(m_waveform.filenameGet().begin());
			break;
		case DESCRIPTION:
			m_waveform.descriptionSet(String(entry.content()));
		//	TODO: Pass message so the keyboard can be updated
			break;
		case COLOR:
			try
				{
				auto c=colorFromString(entry.content());
				m_waveform.keyColorSet(c);
			//	TODO: Pass message so the keyboard can be updated
				}
			catch(...)
				{entry.content(ColorString(m_waveform.keyColorGet()).begin());}
			break;
		case GAIN_TEXT:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{
				m_waveform.gainSet(val_new);
				m_gain_input_slider.value(gain_map_inv(val_new));
				}
			else
				{
				char buffer[16];
				sprintf(buffer,"%.3f",m_waveform.gainGet());
				entry.content(buffer);
				}
			}
			break;
		case GAIN_RANDOM_TEXT:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{
				m_waveform.gainRandomSet(val_new);
				m_gain_random_input_slider.value(gain_random_map_inv(val_new));
				}
			else
				{
				char buffer[16];
				sprintf(buffer,"%.3f",m_waveform.gainRandomGet());
				entry.content(buffer);
				}
			}
			break;
		}
	}

void WaveformEditor::changed(Listbox& lb)
	{
	switch(lb.id())
		{
		case CHANNEL:
			m_waveform.channelSet(lb.selected());
			break;
		}
	}
