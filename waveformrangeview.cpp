#ifdef __WAND__
target[name[waveformrangeview.o] type[object]]
#endif

#include "waveformrangeview.h"
#include "waveform.h"
#include "meansquare.h"
#include "units.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/color.h"
#include "framework/curve.h"
#include "framework/button.h"
#include "framework/label.h"

#include <cstdio>
#include <cstdlib>

WaveformRangeView::EventHandlerPlot::EventHandlerPlot(WaveformRangeView& view):
	r_view(view),r_cursor_x(nullptr),r_cursor_y(nullptr),r_cursor_begin(nullptr)
	{}

void WaveformRangeView::EventHandlerPlot::onCursorXHit(XYPlot& source
	,XYPlot::Cursor& cursor,keymask_t keymask)
	{
	r_cursor_x=&cursor;
	}

void WaveformRangeView::EventHandlerPlot::onCursorYHit(XYPlot& source
	,XYPlot::Cursor& cursor,keymask_t keymask)
	{
	r_cursor_y=&cursor;
	}


void WaveformRangeView::EventHandlerPlot::onMouseMove(XYPlot& source
	,const Curve::Point& point,keymask_t key_mask)
	{
	if(r_cursor_x!=nullptr)
		{
		r_cursor_x->position=point.x;
		r_view.inputSet(r_cursor_x-r_cursor_begin,point.x);
		source.update();
		}

	if(r_cursor_y!=nullptr)
		{
		r_cursor_y->position=point.y;
		if(key_mask&KEYMASK_KEY_SHIFT)
			{
			r_view.autotrim(0);
			r_view.autotrim(1);
			}
		else
			{source.update();}
		}
	}

void WaveformRangeView::EventHandlerPlot::cursorsRefSet(const XYPlot::Cursor& begin)
	{r_cursor_begin=&begin;}

void WaveformRangeView::EventHandlerPlot::onMouseUp(XYPlot& source
	,const Curve::Point& point,keymask_t key_mask)
	{
	if(r_cursor_x!=nullptr)
		{
		r_cursor_x->position=point.x;
		r_view.rangeUpdate(r_cursor_x-r_cursor_begin,point.x);
		source.update();
		}

	r_cursor_x=nullptr;
	r_cursor_y=nullptr;
	}



WaveformRangeView::EventHandlerEntry::EventHandlerEntry(WaveformRangeView& view):
	r_view(view)
	{}

void WaveformRangeView::EventHandlerEntry::onButtonClick(InputEntry& source)
	{
	r_view.autotrim(source.idGet());
	}

void WaveformRangeView::autotrim(unsigned int index)
	{
	auto threshold=dBtoPower(m_plot->cursorYGet(0).position);
	switch(index)
		{
		case 0:
			r_handler->beginAutotrim(*this,threshold);
			break;
		case 1:
			r_handler->endAutotrim(*this,threshold);
			break;
		default:
			break;
		}
	}

void WaveformRangeView::rangeUpdate(unsigned int index,double value)
	{
	uint32_t position=secondsToFrames(value,r_range->sampleRateGet());
	switch(index)
		{
		case 0:
			r_handler->beginUpdate(*this,position);
			break;
		case 1:
			r_handler->endUpdate(*this,position);
			break;
		}
	}

void WaveformRangeView::EventHandlerEntry::onTextChanged(InputEntry& source)
	{
	auto text=source.textGet();
	char* ptr_end;
	auto x=strtod(text,&ptr_end);
	if(ptr_end!=text)
		{r_view.rangeUpdate(source.idGet(),x);}
	}

void WaveformRangeView::EventHandlerEntry::onCommand(BoxVertical& source
	,unsigned int command_id)
	{
	switch(command_id)
		{
		case COMMAND_REVERSE:
			r_view.reverse();
			break;
		}
	}



WaveformRangeView* WaveformRangeView::create(GuiContainer& parent
	,EventHandler& handler)
	{return new WaveformRangeView(parent,handler);}

WaveformRangeView::WaveformRangeView(GuiContainer& parent,EventHandler& handler):
	r_range(nullptr),r_handler(&handler),r_parent(parent)
	,m_plot_handler(*this),m_entry_handler(*this),m_waveform_curve(1024),m_fs(48000)
	{
	m_box_main=BoxVertical::create(parent,&m_entry_handler);
	m_box_main->slaveAssign(*this);
	m_box_main->insertModeSet(BoxVertical::INSERTMODE_LEFT);
	m_label=Label::create(*m_box_main,"Trim:");

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_FILL
		| BoxVertical::INSERTMODE_EXPAND);
	m_plot=XYPlot::create(*m_box_main);

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_END);
	m_box_positions=BoxHorizontal::create(*m_box_main);

	m_entries[0]=InputEntry::create(*m_box_positions,"Begin:","Auto"
		,m_entry_handler,0);

	m_box_positions->insertModeSet(BoxHorizontal::INSERTMODE_FILL
		|BoxHorizontal::INSERTMODE_EXPAND);
	m_swap=Button::create(*m_box_positions,"⇌",EventHandlerEntry::COMMAND_REVERSE);

	m_box_positions->insertModeSet(BoxHorizontal::INSERTMODE_END);
	m_entries[1]=InputEntry::create(*m_box_positions,"End:","Auto"
		,m_entry_handler,1);


	auto c_begin=m_plot->cursorXAdd({-0.5,1.0f/3});
	m_plot->cursorXAdd({0.5,0});
	m_plot->cursorYAdd({-100,0.16f});
	m_plot_handler.cursorsRefSet(m_plot->cursorXGet(c_begin));
	m_plot->eventHandlerSet(m_plot_handler);

	auto nullsignal=Waveform::nullGet();
	waveformSet(nullsignal);
	m_plot->curveAdd(
		{
		 m_waveform_curve.begin()
		,m_waveform_curve.length()
		,COLORS[ColorID::BLUE]
		});
	m_plot->domainSet(
		{
			 {0,-145}
			,{
				framesToSeconds(nullsignal.lengthFull()
					,nullsignal.sampleRateGet())
				,0
			 }
		});
	m_plot->backgroundSet(0);
	}

WaveformRangeView::~WaveformRangeView()
	{
	m_entries[0]->destroy();
	m_swap->destroy();
	m_entries[1]->destroy();
	m_box_positions->destroy();
	m_plot->destroy();
	m_label->destroy();
	m_box_main->slaveRelease();
	}

void WaveformRangeView::destroy()
	{
	delete this;
	}

const GuiHandle& WaveformRangeView::handleNativeGet() const
	{
	return m_box_main->handleNativeGet();
	}

void WaveformRangeView::inputSet(unsigned int index,double x)
	{
	char buffer[32];
	sprintf(buffer,"%.5f",x);
	m_entries[index]->textSet(buffer);
	}

void WaveformRangeView::cursorSet(unsigned int index,double x)
	{
	m_plot->cursorXGet(index).position=x;
	m_plot->update();
	}

void WaveformRangeView::reverse()
	{
	r_handler->reverse(*this);
	}

void WaveformRangeView::waveformSet(Waveform& range)
	{
	r_range=&range;
	double fs=r_range->sampleRateGet();
	auto length_in=r_range->lengthFull();
	ArraySimple<float> meansquare_full(length_in);
	r_handler->waveformRenderPrepare(*this,meansquare_full.begin(),length_in);

		{
	//	Collect values to plot
		auto ptr_dest_begin=m_waveform_curve.begin();
		auto length_out=m_waveform_curve.length();
		uint32_t i=0;
		auto ratio=double(length_in)/length_out;
		auto value=std::max(-145.0f,powerToDb(meansquare_full[0]));
		auto y_min=value;
		auto y_max=value;

		while(i!=length_out)
			{
			auto index_mapped=uint32_t(i*ratio);

			value=std::max(-145.0f,powerToDb(meansquare_full[index_mapped]));
			y_max=std::max(y_max,value);
			y_min=std::min(y_min,value);

			Curve::Point p
				{
				framesToSeconds(index_mapped,fs)
				,value
				};

			*ptr_dest_begin=p;
			++i;
			++ptr_dest_begin;
			}

		if(std::abs(y_max-y_min)<1e-7)
			{
			y_min=-145;
			y_max=0;
			}

		m_plot->cursorYGet(0).position=0.25*y_max+0.75*y_min;
		m_plot->domainSet({{0,y_min},{framesToSeconds(length_in,fs),y_max}});
		}

	cursorsUpdate();
	}

void WaveformRangeView::cursorsUpdate()
	{
	auto fs=r_range->sampleRateGet();
	inputSet(0,framesToSeconds(r_range->offsetBeginGet(),fs));
	cursorSet(0,framesToSeconds(r_range->offsetBeginGet(),fs));
	inputSet(1,framesToSeconds(r_range->offsetEndGet(),fs));
	cursorSet(1,framesToSeconds(r_range->offsetEndGet(),fs));
	}
