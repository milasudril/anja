#ifdef __WAND__
target[name[waveformview.h] type[include]]
dependency[waveformview.o]
#endif

#ifndef WAVEFORMVIEW_H
#define WAVEFORMVIEW_H

#include "framework/widget.h"
#include "framework/xyplot.h"
#include "framework/inputentry.h"
#include "framework/boxvertical.h"

class BoxVertical;
class BoxHorizontal;

class WaveformView:public Widget
	{
	public:
		static WaveformView* create(GuiContainer& parent);

		void destroy();
		void beginSet(double x);
		double beginGet() const;
		void endSet(double x);
		double endGet() const;
		const GuiHandle& handleNativeGet() const;

	private:
		WaveformView(GuiContainer& parent);

		class EventHandlerPlot:public XYPlot::EventHandler
			{
			public:
				EventHandlerPlot(WaveformView& view);
				void onCursorXHit(XYPlot& source,XYPlot::Cursor& cursor
					,keymask_t keymask);
				void onMouseMove(XYPlot& source,const Curve::Point& point
					,keymask_t key_mask);
				void onMouseUp(XYPlot& source,const Curve::Point& point
					,keymask_t key_mask);

			private:
				WaveformView& r_view;
				XYPlot::Cursor* r_cursor;
			};

		class EventHandlerEntry:public InputEntry::EventHandler
			{
			public:
				EventHandlerEntry(WaveformView& view);
				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);

			private:
				WaveformView& r_view;
			};

		class EventHandlerInternal:public BoxVertical::EventHandler
			{
			public:
				EventHandlerInternal(WaveformView& view);
				void onDestroy(BoxVertical& source);

			private:
				WaveformView* r_view;
			};

		GuiContainer& r_parent;
		EventHandlerInternal handler;
		EventHandlerPlot plot_handler;
		EventHandlerEntry entry_handler;
		BoxVertical* m_box_main;
		XYPlot* m_plot;
		BoxHorizontal* m_box_positions;
		InputEntry* m_entry_begin;
		InputEntry* m_entry_end;
	};

#endif
