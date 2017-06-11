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

 WaveformEditor& WaveformEditor::channelName(int index,const char* name)
	{
	m_channel_input.replace(index,name);
	return *this;
	}

WaveformEditor& WaveformEditor::colorPresets(const ColorRGBA* begin,const ColorRGBA* end)
	{
	r_color_presets_begin=begin;
	r_color_presets_end=end;
	if(m_color_dlg)
		{
		if(m_color_dlg)
			{m_color_dlg->widget().presets(begin,end);}
		}
	return *this;
	}


static double gain_map(double x)
	{return 6.0*x + (1-x)*(-72.0);}

static double gain_map_inv(double x)
	{return (x + 72)/(72 + 6);}

static double gain_random_map(double x)
	{return 12.0*x;}

static double gain_random_map_inv(double x)
	{return x/12.0;}

void WaveformEditor::offsets_update()
	{
	auto val=0.0;
	char buffer[32];

	val=static_cast<double>( m_waveform.offset<Waveform::Cursor::BEGIN>() )
		/static_cast<double>( m_waveform.sampleRate() );
	sprintf(buffer,"%.5f",val);
	m_cursor_begin_entry.content(buffer);
	m_plot.cursorX(XYPlot::Cursor{static_cast<double>(val),0.33f,0},0);

	val=static_cast<double>( m_waveform.offset<Waveform::Cursor::END>() )
		/static_cast<double>( m_waveform.sampleRate() );
	sprintf(buffer,"%.5f",val);
	m_cursor_end_entry.content(buffer);
	m_plot.cursorX(XYPlot::Cursor{static_cast<double>(val),0.0f,0},1);

	val=static_cast<double>( m_waveform.offset<Waveform::Cursor::BEGIN_LOOP>() )
		/static_cast<double>( m_waveform.sampleRate() );
	sprintf(buffer,"%.5f",val);
	m_cursor_begin_loop_entry.content(buffer);
	m_plot.cursorX(XYPlot::Cursor{static_cast<double>(val),0.33f,1},2);

	val=static_cast<double>( m_waveform.offset<Waveform::Cursor::END_LOOP>() )
		/static_cast<double>( m_waveform.sampleRate() );
	sprintf(buffer,"%.5f",val);
	m_cursor_end_loop_entry.content(buffer);
	m_plot.cursorX(XYPlot::Cursor{static_cast<double>(val),0.0f,1},3);

	m_swap.state(m_waveform.direction()<0);
	}

void WaveformEditor::cursor_begin_auto() noexcept
	{
	auto threshold=m_plot.cursorY(0).position;
	auto i=std::find_if(m_waveform_db.begin(),m_waveform_db.end(),[threshold](float val)
		{return val>threshold;});
	auto t=static_cast<double>(i - m_waveform_db.begin())*0.5e-3;
	m_waveform.offset<Waveform::Cursor::BEGIN>(t);
	offsets_update();
	}

void WaveformEditor::cursor_end_auto() noexcept
	{
	auto threshold=m_plot.cursorY(0).position;
	auto i_begin=std::reverse_iterator<const float*>(m_waveform_db.end());
	auto i_end=std::reverse_iterator<const float*>(m_waveform_db.begin());
	auto i=std::find_if(i_begin,i_end,[threshold](float val)
		{return val>threshold;});
	auto t=static_cast<double>(i.base() - m_waveform_db.begin())*0.5e-3;
	m_waveform.offset<Waveform::Cursor::END>(t);
	offsets_update();
	}


void WaveformEditor::clicked(OptionList& src,OptionListId id,Checkbox& opt)
	{
	switch(id)
		{
		case OptionListId::OPTIONS:
			if(opt.state())
				{m_waveform.flagSet(opt.id());}
			else
				{m_waveform.flagUnset(opt.id());}
			break;
		}
	}

static void gain_update(const WaveformProxy& wf,TextEntry& e,Slider& s)
	{
	auto g=wf.gain();
	char buffer[16];
	sprintf(buffer,"%.3f",g);
	e.content(buffer);
	s.value(gain_map_inv(g));
	}

static void gain_random_update(const WaveformProxy& wf,TextEntry& e,Slider& s)
	{
	auto g=wf.gainRandom();
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
			break;
		}
	}

static ArraySimple<float> mean_square(const float* begin,const float* end,int length)
	{
	ArraySimple<float> vals_ms(end - begin);
	MeanSquare ms(length);
	ms.compute(begin,vals_ms.begin(),end - begin);
	return std::move(vals_ms);
	}

static ArraySimple<float> decimate(const ArraySimple<float>& src,double dt)
	{
	auto N=static_cast<size_t>( src.length()/dt + 0.5 );
	size_t k=0;
	ArraySimple<float> ret(N);
	std::for_each(ret.begin(),ret.end(),[src,&k,dt](float& val)
		{
		auto pos=static_cast<size_t>( k*dt );
		val=src[pos];
		++k;
		});
	return std::move(ret);
	}

void plot_append(const float* begin,const float* end,double dt,XYPlot& plot)
	{
	ArraySimple<XYPlot::Point> points(end - begin);
	size_t k=0;
	std::transform(begin,end,points.begin(),[&k,dt](float val)
		{
		auto t=static_cast<double>(k)*dt;
		++k;
		return XYPlot::Point{t,static_cast<double>(val)};
		});
	plot.curve(points,0.66f);
	}

static ArraySimple<float> filename_update(const WaveformProxy& waveform,TextEntry& e
	,OptionList& options,XYPlot& plot)
	{
	e.content(waveform.filename().begin());
	options.selected(waveform.flags());
	if(waveform.lengthFull()!=0)
		{
		auto fs=static_cast<double>(waveform.sampleRate());
		auto ms=decimate(mean_square(waveform.beginFull(),waveform.endFull()
			,fs/1000.0),0.5e-3*fs);

		std::transform(ms.begin(),ms.end(),ms.begin(),[](float x)
			{return std::max(powerToDb(x),-145.0f);});

		plot_append(ms.begin(),ms.end(),0.5e-3,plot.curvesRemove());
		return std::move(ms);
		}
	else
		{
		ArraySimple<float> ret(2);
		ret[0]=-145.0f;
		ret[1]=-145.0f;
		plot.curvesRemove();
		return std::move(ret);
		}
	}

static void description_update(const WaveformProxy& waveform,TextEntry& e)
	{
	e.content(waveform.description().begin());
	}

static void color_update(const WaveformProxy& waveform,TextEntry& e)
	{
	e.content(ColorString(waveform.keyColor()).begin());
	}

void WaveformEditor::changed(TextEntry& entry,TextEntryId id)
	{
	switch(id)
		{
		case TextEntryId::FILENAME:
			if(*entry.content()!='\0'
				&& !m_waveform.waveformLoaded(entry.content()))
				{
				try
					{
					m_waveform.waveformLoad(entry.content());
					m_waveform_db=filename_update(m_waveform,entry,m_options_input,m_plot);
					cursor_begin_auto();
					cursor_end_auto();
					m_plot.showAll();
					}
				catch(const Error& err)
					{
					m_err_dlg.reset(new Dialog<Message,DialogOk>(m_box,"Anja"
						,err.message(),Message::Type::ERROR));
					m_err_dlg->callback(*this,0);
					entry.content(m_waveform.filename().begin());
					}
				}
			break;

		case TextEntryId::DESCRIPTION:
			m_waveform.description(String(entry.content()));
			if(r_cb_obj!=nullptr)
				{m_vtable.description_changed(r_cb_obj,*this,m_id);}
			description_update(m_waveform,entry);
			break;

		case TextEntryId::COLOR:
			{
			ColorRGBA c;
			if(colorFromString(entry.content(),c))
				{
				m_waveform.keyColorSet(c);
				if(r_cb_obj!=nullptr)
					{m_vtable.color_changed(r_cb_obj,*this,m_id);}
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
				{m_waveform.offset<Waveform::Cursor::BEGIN>(val_new);}
			offsets_update();
			}
			break;

		case TextEntryId::CURSOR_END:
			{
			double val_new;
			if(convert(entry.content(),val_new))
				{m_waveform.offset<Waveform::Cursor::END>(val_new);}
			offsets_update();
			}
			break;
		}
	}

void WaveformEditor::confirmPositive(Dialog<Message,DialogOk>& dlg,int id)
	{
	m_err_dlg.reset();
	}


void WaveformEditor::clicked(Button& src,ButtonId id)
	{
	switch(id)
		{
		case ButtonId::CURSORS_SWAP:
			{
			m_waveform.reverse();
			offsets_update();
			}
			return;

		case ButtonId::FILENAME_BROWSE:
			{
			std::string temp(m_waveform.filename().begin());
			if(filenameSelect(m_filename,m_waveform.directory().begin(),temp
				,FilenameSelectMode::OPEN,[this](const char* path)
					{return m_waveform.loadPossible(path);},"Wave Audio files"))
				{
				try
					{
					auto flags=m_waveform.flags();
					m_waveform.waveformLoad(temp.c_str());
					m_waveform.flagsSet(flags);
					m_waveform.dirtyClear();
					m_waveform_db=filename_update(m_waveform,m_filename_input,m_options_input,m_plot);
					cursor_begin_auto();
					cursor_end_auto();
					m_plot.showAll();
					}
				catch(const Error& err)
					{
					m_err_dlg.reset(new Dialog<Message,DialogOk>(m_box,"Anja"
						,err.message(),Message::Type::ERROR));
					m_err_dlg->callback(*this,0);
					}
				}
			}
			break;

		case ButtonId::FILENAME_RELOAD:
			try
				{
				m_waveform.waveformLoad(m_waveform.filename().begin());
				m_waveform_db=filename_update(m_waveform,m_filename_input,m_options_input,m_plot);
				cursor_begin_auto();
				cursor_end_auto();
				m_plot.showAll();
				}
			catch(const Error& err)
				{
				m_err_dlg.reset(new Dialog<Message,DialogOk>(m_box,"Anja"
					,err.message(),Message::Type::ERROR));
				m_err_dlg->callback(*this,0);
				}
			break;

		case ButtonId::COLOR_PICK:
			m_color_dlg.reset(new Dialog<ColorPicker>(m_box,"Choose a color"));
			m_color_dlg->callback(*this,0).widget()
				.color(m_waveform.keyColor())
				.presets(r_color_presets_begin,r_color_presets_end);
			break;
		}
	src.state(0);
	}

void WaveformEditor::changed(Listbox& lb,ListboxId id)
	{
	switch(id)
		{
		case ListboxId::CHANNEL:
			m_waveform.channel(lb.selected());
			break;
		}
	}

void WaveformEditor::cursorXMove(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{
	switch(id)
		{
		case PlotId::WAVEFORM:
			switch(index)
				{
				case 0:
					m_waveform.offset<Waveform::Cursor::BEGIN>(plot.cursorX(0).position);
					offsets_update();
					break;

				case 1:
					m_waveform.offset<Waveform::Cursor::END>(plot.cursorX(1).position);
					offsets_update();
					break;

				case 2:
					m_waveform.offset<Waveform::Cursor::BEGIN_LOOP>(plot.cursorX(2).position);
					offsets_update();
					break;

				case 3:
					m_waveform.offset<Waveform::Cursor::END_LOOP>(plot.cursorX(3).position);
					offsets_update();
					break;


				}
			break;
		}
	}

void WaveformEditor::cursorYMove(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{
	switch(id)
		{
		case PlotId::WAVEFORM:
			if(keymask&KEYMASK_KEY_SHIFT && index==0)
				{
				cursor_begin_auto();
				cursor_end_auto();
				}
			break;
		}
	}

void WaveformEditor::cursorXRightclick(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{
	switch(id)
		{
		case PlotId::WAVEFORM:
			switch(index)
				{
				case 0:
					cursor_begin_auto();
					break;
				case 1:
					cursor_end_auto();
					break;
				}
		}
	}

void WaveformEditor::cursorYRightclick(XYPlot& plot,PlotId id,int index,keymask_t keymask)
	{}

void WaveformEditor::dismiss(Dialog<ColorPicker>& win,int id)
	{
	m_color_dlg.reset();
	}

void WaveformEditor::confirmPositive(Dialog<ColorPicker>& dlg,int id)
	{
	m_color_input.content(ColorString(dlg.widget().color()).begin());
	m_waveform.keyColorSet(dlg.widget().color());
	if(r_cb_obj!=nullptr)
		{
		m_vtable.color_changed(r_cb_obj,*this,m_id);
		m_vtable.color_presets_changed(r_cb_obj,m_color_dlg->widget());
		}
	m_color_dlg.reset();
	}

WaveformEditor& WaveformEditor::waveform(const WaveformProxy& waveform)
	{
	auto index_old=m_waveform.index();
	m_waveform=waveform;
	description_update(waveform,m_description_input);
	color_update(waveform,m_color_input);
	gain_update(waveform,m_gain_input_text,m_gain_input_slider);
	gain_random_update(waveform,m_gain_random_input_text,m_gain_random_input_slider);

	m_channel_input.selected(m_waveform.channel());
	m_options_input.selected(waveform.flags());

		{
		m_waveform_db=filename_update(waveform,m_filename_input,m_options_input,m_plot);
		if(waveform.index()!=index_old)
			{m_plot.showAll();}
		m_filename_input.focus();
		}
	offsets_update();

	return *this;
	}

WaveformEditor& WaveformEditor::waveformUpdate()
	{
	//TODO: rerender RMS plot?
	m_plot.showAll();
	return *this;
	}

WaveformEditor& WaveformEditor::channelNames(const String* names_begin,const String* names_end)
	{
	m_channel_input.clear();
	std::for_each(names_begin,names_end,[this](const String& str)
		{m_channel_input.append(str.begin());});
	return *this;
	}

WaveformEditor::WaveformEditor(Container& cnt,const WaveformProxy& waveform
	,const String* channel_names_begin,const String* channel_names_end):
	 r_cb_obj(nullptr)
	,m_waveform(waveform)
	,m_waveform_db(2)
	,m_box(cnt,true)
		,m_filename(m_box.insertMode({2,0}),false)
			,m_filename_label(m_filename.insertMode({2,0}),"Source:")
			,m_filename_input(m_filename.insertMode({2,Box::EXPAND|Box::FILL}))
			,m_filename_browse(m_filename.insertMode({0,0}),"Browse…")
			,m_filename_reload(m_filename.insertMode({2,0}),"↺")
		,m_description(m_box,false)
			,m_description_label(m_description.insertMode({2,0}),"Description:")
			,m_description_input(m_description.insertMode({2,Box::EXPAND|Box::FILL}))
		,m_details(m_box.insertMode({2,Box::EXPAND|Box::FILL}),false)
			,m_details_left(m_details.insertMode({Paned::SHRINK_ALLOWED}),true)
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
						,m_cursor_begin_label(m_cursor_begin,"↦")
						,m_cursor_begin_entry(m_cursor_begin.insertMode({0,Box::EXPAND|Box::FILL}))
					,m_trim_sep_a(m_trim_panel.insertMode({2,0}),true)
					,m_cursor_begin_loop(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_begin_loop_label(m_cursor_begin_loop,"↪")
						,m_cursor_begin_loop_entry(m_cursor_begin_loop.insertMode({2,Box::EXPAND|Box::FILL}))
					,m_trim_sep_b(m_trim_panel.insertMode({2,0}),true)
					,m_swap(m_trim_panel.insertMode({4,Box::EXPAND|Box::FILL}),"⇌")
					,m_trim_sep_c(m_trim_panel.insertMode({2,0}),true)
					,m_cursor_end_loop(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_end_loop_entry(m_cursor_end_loop.insertMode({2,Box::EXPAND|Box::FILL}))
						,m_cursor_end_loop_label(m_cursor_end_loop.insertMode({0,0}),"↩")
					,m_trim_sep_d(m_trim_panel.insertMode({2,0}),true)
					,m_cursor_end(m_trim_panel.insertMode({2,Box::EXPAND|Box::FILL}),false)
						,m_cursor_end_entry(m_cursor_end.insertMode({0,Box::EXPAND|Box::FILL}))
						,m_cursor_end_label(m_cursor_end.insertMode({0,0}),"⇥")
	{
	m_gain_input_text.width(7).small(true).alignment(1.0f);
	m_gain_random_input_text.width(6).small(true).alignment(1.0f);
	m_cursor_begin_entry.width(7);
	m_cursor_begin_loop_entry.width(7);
	m_cursor_end_loop_entry.width(7).alignment(1.0f);
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
	m_cursor_end_entry.callback(*this,TextEntryId::CURSOR_END);
	m_swap.callback(*this,ButtonId::CURSORS_SWAP);

//	Session changed...
	channelNames(channel_names_begin,channel_names_end);

//	Slot changed
	description_update(waveform,m_description_input);
	color_update(waveform,m_color_input);
	gain_update(waveform,m_gain_input_text,m_gain_input_slider);
	gain_random_update(waveform,m_gain_random_input_text,m_gain_random_input_slider);

	m_channel_input.selected(m_waveform.channel());
	m_options_input.selected(waveform.flags());
	m_waveform_db=filename_update(waveform,m_filename_input,m_options_input,m_plot);
	m_plot.showAll();
	m_filename_input.focus();
	offsets_update();
	}
