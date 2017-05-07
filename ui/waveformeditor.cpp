//@	{"targets":[{"name":"waveformeditor.o","type":"object"}]}

#include "waveformeditor.hpp"
#include "filenameselect.hpp"
#include "../sessiondata/session.hpp"
#include "../common/colorstring.hpp"
#include "../common/arraysimple.hpp"
#include "../common/floatconv.hpp"
#include "../common/meansquare.hpp"
#include "../common/units.hpp"

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

static void offset_begin_update(const WaveformView& waveform,TextEntry& e,XYPlot& plot)
	{
	auto val=static_cast<double>( waveform.offsetBeginGet() ) 
		/static_cast<double>( waveform.sampleRateGet() );
	char buffer[32];
	sprintf(buffer,"%.5f",val);
	e.content(buffer);
	plot.cursorX(XYPlot::Cursor{static_cast<double>(val),0.33f},0);
	}

static void offset_end_update(const WaveformView& waveform,TextEntry& e,XYPlot& plot)
	{
	auto val=static_cast<double>( waveform.offsetEndGet() ) 
		/static_cast<double>( waveform.sampleRateGet() );
	char buffer[32];
	sprintf(buffer,"%.5f",val);
	e.content(buffer);
	plot.cursorX(XYPlot::Cursor{val,0.0f},1);
	}

static void cursor_begin_auto(XYPlot& plot,const ArraySimple<float>& wfdb
	,WaveformView& waveform,TextEntry& entry)
	{
	auto threshold=plot.cursorY(0).position;
	auto i=std::find_if(wfdb.begin(),wfdb.end(),[threshold](float val)
		{return val>threshold;});
	waveform.offsetBeginSet(static_cast<int32_t>(i - wfdb.begin()));
	offset_begin_update(waveform,entry,plot);
	}

static void cursor_end_auto(XYPlot& plot,const ArraySimple<float>& wfdb
	,WaveformView& waveform,TextEntry& entry)
	{
	auto threshold=plot.cursorY(0).position;
	auto i_begin=std::reverse_iterator<const float*>(wfdb.end());
	auto i_end=std::reverse_iterator<const float*>(wfdb.begin());
	auto i=std::find_if(i_begin,i_end,[threshold](float val)
		{return val>threshold;});
	waveform.offsetEndSet(static_cast<int32_t>(i.base() - wfdb.begin()));
	offset_end_update(waveform,entry,plot);
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

static void gain_update(const WaveformView& wf,TextEntry& e,Slider& s)
	{
	auto g=wf.gainGet();
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
	s.value(gain_map_inv(g));
	}

static void gain_random_update(const WaveformView& wf,TextEntry& e,Slider& s)
	{
	auto g=wf.gainRandomGet();
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
	s.value(gain_random_map_inv(g));
	}

void WaveformEditor::changed(Slider& slider,SliderId id)
	{
	switch(id)
		{
		case SliderId::GAIN:
			m_waveform.gainSet( gain_map(slider.value()) );
			gain_update(m_waveform,m_gain_input_text,slider);
			break;

		case SliderId::GAIN_RANDOM:
			m_waveform.gainRandomSet( gain_random_map(slider.value()) );
			gain_random_update(m_waveform,m_gain_random_input_text,slider);
		}
	}

static ArraySimple<float> mean_square(const float* begin,const float* end,int length)
	{
	ArraySimple<float> vals_ms(end - begin);
	MeanSquare ms(length);
	ms.compute(begin,vals_ms.begin(),end - begin);
	return std::move(vals_ms);
	}

void plot_append(const float* begin,const float* end,int fs,XYPlot& plot)
	{
	ArraySimple<XYPlot::Point> points(end - begin);
	size_t k=0;
	std::transform(begin,end,points.begin(),[&k,fs](float val)
		{
		auto t=static_cast<double>(k)/static_cast<double>(fs);
		++k;
		return XYPlot::Point{t,static_cast<double>(val)};
		});
	plot.curve(points.begin(),points.end(),0.66f);
	}

static ArraySimple<float> filename_update(const WaveformView& waveform,TextEntry& e,XYPlot& plot)
	{
	e.content(waveform.filenameGet().begin());
	auto ms=mean_square(waveform.beginFull(),waveform.endFull()
		,waveform.sampleRateGet()/1000);
	std::transform(ms.begin(),ms.end(),ms.begin(),[](float x)
		{return std::max(powerToDb(x),-145.0f);});

	plot_append(ms.begin(),ms.end(),waveform.sampleRateGet(),plot.curvesRemove());
	return std::move(ms);
	}

static void description_update(const WaveformView& waveform,TextEntry& e)
	{
	e.content(waveform.descriptionGet().begin());
	}

static void color_update(const WaveformView& waveform,TextEntry& e)
	{
	e.content(ColorString(waveform.keyColorGet()).begin());
	}

void WaveformEditor::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::FILENAME:
		//	TODO: Pass message. We need to know session directory before trying
		//	to load a new file. After the filename has been successfully loaded,
		//	show its canonical path.
			filename_update(m_waveform,entry,m_plot);
			break;

		case TextEntryId::DESCRIPTION:
			m_waveform.descriptionSet(String(entry.content()));
		//	TODO: Pass message so the keyboard can be updated
			description_update(m_waveform,entry);
			break;

		case TextEntryId::COLOR:
			{
			ColorRGBA c;
			if(colorFromString(entry.content(),c))
				{
				m_waveform.keyColorSet(c);
			//	TODO: Pass message so the keyboard can be updated
				}
			color_update(m_waveform,entry);
			}
			break;

		case TextEntryId::GAIN:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_waveform.gainSet(val_new);}
			gain_update(m_waveform,entry,m_gain_input_slider);
			}
			break;

		case TextEntryId::GAIN_RANDOM:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_waveform.gainRandomSet(val_new);}
			gain_random_update(m_waveform,entry,m_gain_random_input_slider);
			}
			break;

		case TextEntryId::CURSOR_BEGIN:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_waveform.offsetBeginSet(val_new);}
			offset_begin_update(m_waveform,entry,m_plot);
			}
			break;

		case TextEntryId::CURSOR_END:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_waveform.offsetEndSet(val_new);}
			offset_begin_update(m_waveform,entry,m_plot);
			}
			break;
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
			offset_begin_update(m_waveform,m_cursor_begin_entry,m_plot);
			offset_end_update(m_waveform,m_cursor_end_entry,m_plot);
			}
			break;

		case ButtonId::CURSOR_BEGIN_AUTO:
			cursor_begin_auto(m_plot,m_waveform_db,m_waveform,m_cursor_begin_entry);
			break;

		case ButtonId::CURSOR_END_AUTO:
			cursor_end_auto(m_plot,m_waveform_db,m_waveform,m_cursor_end_entry);
			break;

		case ButtonId::FILENAME_BROWSE:
			{
			std::string temp(m_waveform.filenameGet().begin());
			if(filenameSelect(m_filename,temp,FilenameSelectMode::OPEN
				,[this](const char* path)
					{return m_waveform.loadPossible(path);},"Wave Audio files"))
				{
			//	TODO: Pass message. We need to know session directory before trying
			//	to load a new file. After the filename has been successfully loaded,
			//	show its canonical path.
				filename_update(m_waveform,m_filename_input,m_plot);
				}
			}
			break;

		case ButtonId::COLOR_PICK:
			m_color_dlg.reset(new Window("Choose color",&m_box));
			m_color_dlg->callback(*this,PopupId::COLOR_SELECT).modal(true).show();
			break;
		}
	src.state(0);
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

void WaveformEditor::cursorX(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{
	switch(id)
		{
		case PlotId::WAVEFORM:
			switch(index)
				{
				case 0:
					m_waveform.offsetBeginSet(plot.cursorX(0).position);
					offset_begin_update(m_waveform,m_cursor_begin_entry,plot);
					break;
				case 1:
					m_waveform.offsetEndSet(plot.cursorX(1).position);
					offset_end_update(m_waveform,m_cursor_end_entry,plot);
					break;
				}
			break;
		}
	}

void WaveformEditor::cursorY(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{
	switch(id)
		{
		case PlotId::WAVEFORM:
			if(keymask&KEYMASK_KEY_SHIFT && index==0)
				{
				cursor_begin_auto(m_plot,m_waveform_db,m_waveform,m_cursor_begin_entry);
				cursor_end_auto(m_plot,m_waveform_db,m_waveform,m_cursor_end_entry);
				}
			break;
		}
	}

void WaveformEditor::closing(Window& win,PopupId id)
	{
	m_color_dlg.reset();
	}

WaveformEditor::WaveformEditor(Container& cnt,const WaveformView& waveform
	,const ArraySimple<String>& channel_names):
	 m_waveform(waveform)
	,m_waveform_db(2)
	,m_box(cnt,true)
		,m_filename(m_box.insertMode({1,0}),false)
			,m_filename_label(m_filename.insertMode({2,0}),"Source:")
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}))
			,m_filename_browse(m_filename.insertMode({0,0}),"Browse")
			,m_filename_reload(m_filename,"↺")
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description:")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}))
		,m_details(m_box.insertMode({2,Box::EXPAND|Box::FILL}),false)
			,m_details_left(m_details.insertMode({Paned::SHRINK_ALLOWED}),true)
				,m_color(m_details_left.insertMode({2,0}),false)
					,m_color_label(m_color.insertMode({2,0}),"Color:")
					,m_color_input(m_color.insertMode({2,Box::EXPAND|Box::FILL}))
					,m_color_pick(m_color.insertMode({0,0}),"…")
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
						,m_cursor_begin_label(m_cursor_begin,"Begin:")
						,m_cursor_begin_entry(m_cursor_begin.insertMode({2,Box::EXPAND|Box::FILL}))
						,m_cursor_begin_auto(m_cursor_begin.insertMode({0,0}),"Auto")
					,m_swap(m_trim_panel.insertMode({2,Box::EXPAND}),"⇌")
					,m_cursor_end(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_end_label(m_cursor_end,"End:")
						,m_cursor_end_entry(m_cursor_end.insertMode({2,Box::EXPAND|Box::FILL}))
						,m_cursor_end_auto(m_cursor_end.insertMode({0,0}),"Auto")
	{
	m_gain_input_text.width(7).small(true).alignment(1.0f);
	m_gain_random_input_text.width(6).small(true).alignment(1.0f);
	m_cursor_begin_entry.width(7);
	m_cursor_end_entry.width(7).alignment(1.0f);
	m_plot.cursorY(XYPlot::Cursor{-70.0,0.14f});
	m_options_input.append(waveform.flagNames());

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
	m_plot.callback(*this,PlotId::WAVEFORM);
	m_cursor_begin_entry.callback(*this,TextEntryId::CURSOR_BEGIN);
	m_cursor_begin_auto.callback(*this,ButtonId::CURSOR_BEGIN_AUTO);
	m_cursor_end_entry.callback(*this,TextEntryId::CURSOR_END);
	m_cursor_end_auto.callback(*this,ButtonId::CURSOR_END_AUTO);
	m_swap.callback(*this,ButtonId::CURSORS_SWAP);

//	Session changed...
	std::for_each(channel_names.begin(),channel_names.end(),[this](const String& str)
		{m_channel_input.append(str.begin());});

//	Slot changed
	description_update(waveform,m_description_input);
	color_update(waveform,m_color_input);
	gain_update(waveform,m_gain_input_text,m_gain_input_slider);
	gain_random_update(waveform,m_gain_random_input_text,m_gain_random_input_slider);

	m_channel_input.selected(m_waveform.channelGet());
	m_options_input.selected(waveform.flagsGet());

		{
		m_waveform_db=filename_update(waveform,m_filename_input,m_plot);
	//	Only when slot changed (Not when user selects keyboard view and clicks the same slot)
		m_plot.showAll();
		}
	offset_begin_update(waveform,m_cursor_begin_entry,m_plot);
	offset_end_update(waveform,m_cursor_end_entry,m_plot);
	}
