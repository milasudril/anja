#ifdef __WAND__
target[name[waveformview.o] type[object]]
#endif

#include "waveformview.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"

#include <cstdio>
#include <cstdlib>

WaveformView::EventHandlerPlot::EventHandlerPlot(WaveformView& view):
	r_view(view),r_cursor(nullptr),r_cursor_begin(nullptr)
	{}

void WaveformView::EventHandlerPlot::onCursorXHit(XYPlot& source
	,XYPlot::Cursor& cursor,keymask_t keymask)
	{
	r_cursor=&cursor;
	}

void WaveformView::EventHandlerPlot::onMouseMove(XYPlot& source
	,const Curve::Point& point,keymask_t key_mask)
	{
	if(r_cursor!=nullptr)
		{
		r_cursor->position=point.x;
		r_view.inputSet(r_cursor-r_cursor_begin,point.x);
		source.update();
		}
	}

void WaveformView::EventHandlerPlot::cursorsRefSet(const XYPlot::Cursor& begin)
	{r_cursor_begin=&begin;}

void WaveformView::EventHandlerPlot::onMouseUp(XYPlot& source
	,const Curve::Point& point,keymask_t key_mask)
	{
	r_cursor=nullptr;
	}



WaveformView::EventHandlerEntry::EventHandlerEntry(WaveformView& view):
	r_view(view)
	{}

void WaveformView::EventHandlerEntry::onButtonClick(InputEntry& source)
	{
	switch(source.idGet())
		{
		case 0:
			printf("Find start position\n");
			break;
		case 1:
			printf("Find end position\n");
			break;
		default:
			break;
		}
	}

void WaveformView::EventHandlerEntry::onTextChanged(InputEntry& source)
	{
	auto text=source.textGet();
	char* ptr_end;
	auto x=strtod(text,&ptr_end);
	if(ptr_end!=text)
		{r_view.cursorSet(source.idGet(),x);}
	}



WaveformView* WaveformView::create(GuiContainer& parent)
	{return new WaveformView(parent);}

WaveformView::WaveformView(GuiContainer& parent):r_parent(parent)
	,plot_handler(*this),entry_handler(*this)
	{
	m_box_main=BoxVertical::create(parent);
	m_box_main->slaveAssign(*this);

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_FILL
		| BoxVertical::INSERTMODE_EXPAND);
	m_plot=XYPlot::create(*m_box_main);

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_END);
	m_box_positions=BoxHorizontal::create(*m_box_main);

	m_entries[0]=InputEntry::create(*m_box_positions,"Begin:","Auto"
		,entry_handler,0);

	m_box_positions->insertModeSet(BoxVertical::INSERTMODE_END);
	m_entries[1]=InputEntry::create(*m_box_positions,"End:","Auto"
		,entry_handler,1);

	auto c_begin=m_plot->cursorXAdd({-0.5,1.0f/3});
	m_plot->cursorXAdd({0.5,0});
	plot_handler.cursorsRefSet(m_plot->cursorXGet(c_begin));
	m_plot->eventHandlerSet(plot_handler);
	}

void WaveformView::destroy()
	{
	m_entries[0]->destroy();
	m_entries[1]->destroy();
	m_box_positions->destroy();
	m_plot->destroy();
	m_box_main->slaveRelease();
	delete this;
	}

const GuiHandle& WaveformView::handleNativeGet() const
	{
	return m_box_main->handleNativeGet();
	}

void WaveformView::inputSet(size_t index,double x)
	{
	char buffer[32];
	sprintf(buffer,"%.5f",x);
	m_entries[index]->textSet(buffer);
	}

void WaveformView::cursorSet(size_t index,double x)
	{
	m_plot->cursorXGet(index).position=x;
	m_plot->update();
	}

/*
		void beginSet(double x);
		double beginGet() const;
		void endSet(double x);
		double endGet() const;*/
