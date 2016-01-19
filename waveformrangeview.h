#ifdef __WAND__
target[name[waveformrangeview.h] type[include]]
dependency[waveformrangeview.o]
#endif

#ifndef WAVEFORMRANGEVIEW_H
#define WAVEFORMRANGEVIEW_H

#include "framework/widget.h"
#include "framework/xyplot.h"
#include "framework/inputentry.h"
#include "framework/array_simple.h"
#include <valarray>

class BoxVertical;
class BoxHorizontal;
class Waveform;

class WaveformRangeView:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void beginUpdate(WaveformRangeView& view
					,uint32_t position)=0;

				virtual void endUpdate(WaveformRangeView& view
					,uint32_t position)=0;

				virtual void reverse(WaveformRangeView& view)=0;

				virtual void beginAutotrim(WaveformRangeView& view,float threshold)=0;

				virtual void endAutotrim(WaveformRangeView& view,float threshold)=0;

				virtual void waveformRenderPrepare(WaveformRangeView& view
					,float* buffer_out,size_t length)=0;
			};

		static WaveformRangeView* create(GuiContainer& parent,EventHandler& handler);

		void waveformSet(Waveform& range);

		Waveform& waveformGet()
			{return *r_range;}

		void cursorsUpdate();

		void eventHandlerSet(EventHandler& handler)
			{r_handler=&handler;}

		void destroy();
		const GuiHandle& handleNativeGet() const;

	private:
		friend class EventHandlerPlot;
		friend class EventHandlerEntry;

		WaveformRangeView(GuiContainer& parent,EventHandler& handler);
		~WaveformRangeView();

		class EventHandlerPlot:public XYPlot::EventHandler
			{
			public:
				EventHandlerPlot(WaveformRangeView& view);
				void onCursorXHit(XYPlot& source,XYPlot::Cursor& cursor
					,keymask_t keymask);
				void onCursorYHit(XYPlot& source,XYPlot::Cursor& cursor
					,keymask_t keymask);
				void onMouseMove(XYPlot& source,const Curve::Point& point
					,keymask_t key_mask);
				void onMouseUp(XYPlot& source,const Curve::Point& point
					,keymask_t key_mask);
				void cursorsRefSet(const XYPlot::Cursor& begin);
				void cursorsRefSet(XYPlot::Cursor&&)=delete;

			private:
				WaveformRangeView& r_view;
				XYPlot::Cursor* r_cursor_x;
				XYPlot::Cursor* r_cursor_y;
				const XYPlot::Cursor* r_cursor_begin;
			};

		class EventHandlerEntry:public InputEntry::EventHandler
			,public Button::EventHandler
			{
			public:
				EventHandlerEntry(WaveformRangeView& view);
				void onButtonClick(InputEntry& source);
				void onTextChanged(InputEntry& source);
				void onActionPerform(Button& source);

			private:
				WaveformRangeView& r_view;
			};

		void inputSet(unsigned int index,double x);
		void cursorSet(unsigned int index,double x);
		void autotrim(unsigned int index);
		void rangeUpdate(unsigned int index, double position);
		void reverse();

		Waveform* r_range;
		EventHandler* r_handler;

		EventHandlerPlot m_plot_handler;
		EventHandlerEntry m_entry_handler;
		BoxVertical* m_box_main;
		Label* m_label;
		XYPlot* m_plot;
		BoxHorizontal* m_box_positions;
		InputEntry* m_entries[2];
		Button* m_swap;

		ArraySimple<Curve::Point> m_waveform_curve;
		double m_fs;
	};

#endif
