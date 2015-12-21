#ifdef __WAND__
target[name[waveformview.o] type[object]]
#endif

#include "waveformview.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"

#include <cstdio>

WaveformView::EventHandlerInternal::EventHandlerInternal(WaveformView& view):
	r_view(&view)
	{}



WaveformView::EventHandlerPlot::EventHandlerPlot(WaveformView& view):
	r_view(view)
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
		}
	}

void WaveformView::EventHandlerPlot::onMouseUp(XYPlot& source
	,const Curve::Point& point,keymask_t key_mask)
	{
	r_cursor=nullptr;
	}



WaveformView::EventHandlerEntry::EventHandlerEntry(WaveformView& view):
	r_view(view)
	{}

void WaveformView::EventHandlerEntry::onButtonClick(InputEntry& source)
	{printf("OMG a button\n");}

void WaveformView::EventHandlerEntry::onTextChanged(InputEntry& source)
	{printf("%s\n",source.textGet());}



WaveformView* WaveformView::create(GuiContainer& parent)
	{return new WaveformView(parent);}

WaveformView::WaveformView(GuiContainer& parent):r_parent(parent)
	,handler(*this),plot_handler(*this),entry_handler(*this)
	{
	m_box_main=BoxVertical::create(parent);
	m_box_main->slaveAssign(*this);

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_FILL
		| BoxVertical::INSERTMODE_EXPAND);
	m_plot=XYPlot::create(*m_box_main);

	m_box_main->insertModeSet(BoxVertical::INSERTMODE_END);
	m_box_positions=BoxHorizontal::create(*m_box_main);

	m_entry_begin=InputEntry::create(*m_box_positions,"Begin:","Auto",entry_handler);

	m_box_positions->insertModeSet(BoxVertical::INSERTMODE_END);
	m_entry_end=InputEntry::create(*m_box_positions,"End:","Auto",entry_handler);
	}

void WaveformView::destroy()
	{
	printf("WaveformView %p\n",this);
	m_entry_end->destroy();
	m_entry_begin->destroy();
	m_box_positions->destroy();
	m_plot->destroy();
	m_box_main->slaveRelease();
	delete this;
	}

const GuiHandle& WaveformView::handleNativeGet() const
	{
	return m_box_main->handleNativeGet();
	}

/*
		void beginSet(double x);
		double beginGet() const;
		void endSet(double x);
		double endGet() const;*/
