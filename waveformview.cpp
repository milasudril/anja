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

void WaveformView::EventHandlerInternal::onDestroy()
	{delete r_view;}



WaveformView::EventHandlerPlot::EventHandlerPlot(WaveformView& view):
	r_view(view)
	{}

void WaveformView::EventHandlerPlot::onCursorXHit(XYPlot::Cursor& cursor,keymask_t keymask)
	{
	r_cursor=&cursor;
	}

void WaveformView::EventHandlerPlot::onMouseMove(const Curve::Point& point,keymask_t key_mask)
	{
	if(r_cursor!=nullptr)
		{
		r_cursor->position=point.x;
		}
	}

void WaveformView::EventHandlerPlot::onMouseUp(const Curve::Point& point,keymask_t key_mask)
	{
	r_cursor=nullptr;
	}



WaveformView::EventHandlerEntry::EventHandlerEntry(WaveformView& view):
	r_view(view)
	{}

void WaveformView::EventHandlerEntry::onButtonClick()
	{printf("OMG a button\n");}

void WaveformView::EventHandlerEntry::onTextChanged(const char* value)
	{printf("%s\n",value);}

void WaveformView::EventHandlerEntry::inputEntrySet(InputEntry& entry)
	{}



WaveformView* WaveformView::create(GuiContainer& parent)
	{return new WaveformView(parent);}

WaveformView::WaveformView(GuiContainer& parent):r_parent(parent)
	,handler(*this),plot_handler(*this),entry_handler(*this)
	{
	m_box_main=BoxVertical::create(parent);

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
	r_parent.componentRemove(*m_box_main);
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
