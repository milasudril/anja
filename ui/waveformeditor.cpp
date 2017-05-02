//@	{"targets":[{"name":"waveformeditor.o","type":"object"}]}

#include "waveformeditor.hpp"
#include "../sessiondata/session.hpp"
#include "../common/colorstring.hpp"
#include "../common/arraysimple.hpp"
#include "../common/floatconv.hpp"

#include <cstdio>

using namespace Anja;

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
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}))
			,m_filename_browse(m_filename.insertMode({2,0}),"Browse")
			,m_filename_reload(m_filename,"↺")
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description:")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}))
		,m_details(m_box.insertMode({2,Box::EXPAND|Box::FILL}),false)
			,m_details_left(m_details.insertMode({Paned::SHRINK_ALLOWED|Paned::RESIZE|Paned::SCROLL_ALLOWED}),true)
				,m_color(m_details_left.insertMode({2,0}),false)
					,m_color_label(m_color.insertMode({2,0}),"Color:")
					,m_color_input(m_color.insertMode({2,Box::EXPAND|Box::FILL}))
					,m_color_pick(m_color.insertMode({2,0}),"…")
				,m_channel(m_details_left,false)
					,m_channel_label(m_channel.insertMode({2,0}),"Channel:")
					,m_channel_input(m_channel.insertMode({2,Box::EXPAND|Box::FILL}))
				,m_gain(m_details_left,false)
					,m_gain_label(m_gain.insertMode({2,0}),"Gain/dBFS:")
					,m_gain_input(m_gain.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_gain_input_text(m_gain_input.insertMode({0,0}))
						,m_gain_input_slider(m_gain_input.insertMode({0,Box::EXPAND|Box::FILL}),false)
				,m_gain_random(m_details_left,false)
					,m_gain_random_label(m_gain_random.insertMode({2,0}),"Gain random/dBFS:")
					,m_gain_random_input(m_gain_random.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_gain_random_input_text(m_gain_random_input.insertMode({0,0}))
						,m_gain_random_input_slider(m_gain_random_input.insertMode({0,Box::EXPAND|Box::FILL}),false)
				,m_options(m_details_left,false)
					,m_options_label(m_options.insertMode({2,0}),"Options:")
				,m_options_input(m_details_left.insertMode({0,Box::EXPAND|Box::FILL}),true)
			,m_details_right(m_details.insertMode({Paned::SHRINK_ALLOWED|Paned::RESIZE}),true)
				,m_plot(m_details_right.insertMode({2,Box::EXPAND|Box::FILL}))
				,m_trim_panel(m_details_right.insertMode({0,0}),false)
					,m_cursor_begin(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_begin_label(m_cursor_begin,"Begin")
						,m_cursor_begin_entry(m_cursor_begin.insertMode({4,Box::EXPAND|Box::FILL}))
						,m_cursor_begin_auto(m_cursor_begin.insertMode({0,0}),"Auto")
					,m_swap(m_trim_panel.insertMode({2,Box::EXPAND}),"⇌")
					,m_cursor_end(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_end_label(m_cursor_end,"End")
						,m_cursor_end_entry(m_cursor_end.insertMode({4,Box::EXPAND|Box::FILL}))
						,m_cursor_end_auto(m_cursor_end.insertMode({0,0}),"Auto")
	{
	m_gain_input_text.width(7).small(true).alignment(1.0f);
	m_gain_random_input_text.width(6).small(true).alignment(1.0f);
	m_cursor_begin_entry.width(7);
	m_cursor_end_entry.width(7).alignment(1.0f);

	m_filename_input.callback(*this,TextEntryId::FILENAME);
	m_filename_browse.callback(*this,ButtonId::FILENAME_BROWSE);
	m_filename_reload.callback(*this,ButtonId::FILENAME_RELOAD);
	m_description_input.callback(*this,TextEntryId::DESCRIPTION);
	m_color_input.callback(*this,TextEntryId::COLOR);
	m_color_pick.callback(*this,ButtonId::COLOR_PICK);
	m_channel_input.callback(*this,ListboxId::CHANNEL);
	m_gain_input_slider.callback(*this,SliderId::GAIN);
	m_gain_input_text.callback(*this,TextEntryId::GAIN);
	m_gain_random_input_slider.callback(*this,SliderId::GAIN_RANDOM);
	m_gain_random_input_text.callback(*this,TextEntryId::GAIN_RANDOM);
	m_options_input.callback(*this,OptionListId::OPTIONS);
//	TODO: Add m_plot here...
	m_cursor_begin_entry.callback(*this,TextEntryId::CURSOR_BEGIN);
	m_cursor_end_entry.callback(*this,TextEntryId::CURSOR_END);
	m_swap.callback(*this,ButtonId::CURSORS_SWAP);

	std::for_each(channel_names.begin(),channel_names.end(),[this](const String& str)
		{m_channel_input.append(str.begin());});


	m_filename_input.content(waveform.filenameGet().begin());
	m_description_input.content(waveform.descriptionGet().begin());
	m_color_input.content(ColorString(waveform.keyColorGet()).begin());
	m_gain_input_slider.value(gain_map_inv(waveform.gainGet()));
	changed(m_gain_input_slider,SliderId::GAIN);
	m_gain_random_input_slider.value(gain_random_map_inv(waveform.gainRandomGet()));
	changed(m_gain_random_input_slider,SliderId::GAIN_RANDOM);

	m_channel_input.selected(m_waveform.channelGet());
	m_options_input.append(waveform.flagNames());
	m_options_input.selected(waveform.flagsGet());

	char buffer[16];
	sprintf(buffer,"%d",waveform.offsetBeginGet());
	m_cursor_begin_entry.content(buffer);
	sprintf(buffer,"%d",waveform.offsetEndGet());
	m_cursor_end_entry.content(buffer);


//	Test stuff
		{
		ArrayDynamicShort<XYPlot::Point> points;
		double dx=1.0/128;
		for(int k=-128;k<=128;++k)
			{points.append({dx*k,sin(2.0*acos(-1.0)*k*dx)});}

		m_plot.curve(points.begin(),points.end(),0.0f);
		}
	}

void WaveformEditor::clicked(Button& src,ButtonId id)
	{
	switch(id)
		{
		case ButtonId::CURSORS_SWAP:
			{
			auto end=m_waveform.offsetBeginGet();
			auto begin=m_waveform.offsetEndGet();
			m_waveform.offsetBeginSet(begin);
			m_waveform.offsetEndSet(end);

			char buffer[16];
			sprintf(buffer,"%d",begin);
			m_cursor_begin_entry.content(buffer);
			sprintf(buffer,"%d",end);
			m_cursor_end_entry.content(buffer);
		//	TODO: Update plot
			}
			break;
		}
	src.state(0);
	}

void WaveformEditor::clicked(OptionList<WaveformEditor,OptionListId>& src
	,Checkbox& opt,int bit)
	{
	switch(src.id())
		{
		case OptionListId::OPTIONS:
			if(opt.state())
				{m_waveform.flagSet(bit);}
			else
				{m_waveform.flagsUnset(bit);}
			break;
		}
	}

void WaveformEditor::changed(Slider& slider,SliderId id)
	{
	switch(id)
		{
		case SliderId::GAIN:
			{
			m_waveform.gainSet( gain_map(slider.value()) );
			char buffer[16];
			sprintf(buffer,"%.3f",m_waveform.gainGet());
			m_gain_input_text.content(buffer);
			}
			break;
		case SliderId::GAIN_RANDOM:
			{
			m_waveform.gainRandomSet( gain_random_map(slider.value()) );
			char buffer[16];
			sprintf(buffer,"%.3f",m_waveform.gainRandomGet());
			m_gain_random_input_text.content(buffer);
			}
			break;
		}
	}

void WaveformEditor::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::FILENAME:
		//	TODO: Pass message. We need to know session directory before trying
		//	to load a new file. After the filename has been successfully loaded,
		//	show its canonical path.
			entry.content(m_waveform.filenameGet().begin());
			break;

		case TextEntryId::DESCRIPTION:
			m_waveform.descriptionSet(String(entry.content()));
		//	TODO: Pass message so the keyboard can be updated
			break;

		case TextEntryId::COLOR:
			try
				{
				auto c=colorFromString(entry.content());
				m_waveform.keyColorSet(c);
			//	TODO: Pass message so the keyboard can be updated
				}
			catch(...)
				{entry.content(ColorString(m_waveform.keyColorGet()).begin());}
			break;

		case TextEntryId::GAIN:
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

		case TextEntryId::GAIN_RANDOM:
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

		case TextEntryId::CURSOR_BEGIN:
			{
			auto x=atoi(m_cursor_begin_entry.content());
			m_waveform.offsetBeginSet(x);
			x=m_waveform.offsetBeginGet();
			char buffer[16];
			sprintf(buffer,"%d",x);
			m_cursor_begin_entry.content(buffer);
			}
			break;

		case TextEntryId::CURSOR_END:
			{
			auto x=atoi(m_cursor_begin_entry.content());
			m_waveform.offsetEndSet(x);
			x=m_waveform.offsetBeginGet();
			char buffer[16];
			sprintf(buffer,"%d",x);
			m_cursor_end_entry.content(buffer);
			}
			break;
		}
	}

void WaveformEditor::changed(Listbox& lb,ListboxId id)
	{
	switch(id)
		{
		case ListboxId::CHANNEL:
			m_waveform.channelSet(lb.selected());
			break;
		}
	}
