#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "framework/window.h"
#include "framework/boxvertical.h"
#include "framework/boxhorizontal.h"
#include "framework/inputentry.h"
#include "framework/keyboardview.h"
#include "framework/keyboardlayout.h"
#include "framework/xyplot.h"

class XYPlotHandler:public XYPlot::EventHandler
	{
	public:
		XYPlotHandler(XYPlot& plot):r_plot(plot),r_cursor_x(nullptr)
			,r_cursor_y(nullptr)
			{}

		void onCursorXHit(XYPlot::Cursor& cursor,keymask_t keymask)
			{
			r_cursor_x=&cursor;
			}

		void onCursorYHit(XYPlot::Cursor& cursor,keymask_t keymask)
			{
			r_cursor_y=&cursor;
			}

		void onMouseMove(const Curve::Point& point,keymask_t key_mask)
			{
			bool dirty=0;
			if(r_cursor_x!=nullptr)
				{
				r_cursor_x->position=point.x;
				dirty=1;
				}

			if(r_cursor_y!=nullptr)
				{
				r_cursor_y->position=point.y;
				dirty=1;
				}

			if(dirty)
				{r_plot.update();}
			}

		void onMouseUp(const Curve::Point& point,keymask_t key_mask)
			{
			r_cursor_x=nullptr;
			r_cursor_y=nullptr;
			}

	private:
		XYPlot& r_plot;
		XYPlot::Cursor* r_cursor_x;
		XYPlot::Cursor* r_cursor_y;
	};

int main()
	{
	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");
	auto panels=BoxVertical::create(*mainwin);

	panels->insertModeSet(BoxVertical::INSERTMODE_FILL | BoxVertical::INSERTMODE_EXPAND);
	auto xyplot=XYPlot::create(*panels);

	panels->insertModeSet(BoxVertical::INSERTMODE_END);
	auto panels2=BoxHorizontal::create(*panels);
	auto input_begin=InputEntry::create(*panels2,"Begin:","Auto");
	panels2->insertModeSet(BoxVertical::INSERTMODE_END);
	auto input_end=InputEntry::create(*panels2,"End:","Auto");

	xyplot->cursorXAdd({0,0.0f});
	xyplot->cursorYAdd({0,1.0f/3});
	XYPlotHandler handler(*xyplot);
	xyplot->eventHandlerSet(handler);

	return 0;
	}
