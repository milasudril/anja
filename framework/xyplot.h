#ifdef __WAND__
target[name[xyplot.h] type[include]]
dependency[xyplot.o]
#endif

#ifndef XYPLOT_H
#define XYPLOT_H

#include "widget.h"
#include "keymask.h"
#include "curve.h"
#include <cstdint>
#include <cstddef>

class GuiContainer;

class XYPlot:public Widget
	{
	public:
		struct Cursor
			{
			double position;
			float hue;
			};


		class EventHandler
			{
			public:
				virtual void onMouseMove(XYPlot& source
					,const Curve::Point& point,keymask_t key_mask)
					{}

				virtual void onMouseDown(XYPlot& source
					,const Curve::Point& point,keymask_t key_mask)
					{}

				virtual void onMouseUp(XYPlot& source
					,const Curve::Point& point,keymask_t key_mask)
					{}

				virtual void onKeyDown(XYPlot& source,uint8_t scancode)
					{}

				virtual void onKeyUp(XYPlot& source,uint8_t scancode)
					{}

				virtual void onCursorXHit(XYPlot& source,Cursor& cursor
					,keymask_t key_mask)
					{}

				virtual void onCursorYHit(XYPlot& source,Cursor& cursor
					,keymask_t key_mask)
					{}
			};

		static XYPlot* create(GuiContainer& parent)
			{return create(parent,s_default_handler);}

		static XYPlot* create(GuiContainer& parent,EventHandler& handler);

		virtual void backgroundSet(bool light)=0;
		virtual void update()=0;
		virtual void eventHandlerSet(EventHandler& handler)=0;
		virtual void curveAdd(const Curve& curve)=0;
		virtual void domainSet(const Curve::Domain& domain)=0;
		virtual const Curve::Domain& domainGet() const=0;

		virtual size_t cursorXAdd(const Cursor& cursor)=0;
		virtual void cursorXRemove(size_t n)=0;
		virtual void cursorXSet(size_t n,const Cursor& cursor)=0;
		virtual Cursor& cursorXGet(size_t n)=0;
		virtual const Cursor& cursorXGet(size_t n) const=0;

		virtual size_t cursorYAdd(const Cursor& cursor)=0;
		virtual void cursorYRemove(size_t n)=0;
		virtual void cursorYSet(size_t n,const Cursor& cursor)=0;
		virtual Cursor& cursorYGet(size_t n)=0;
		virtual const Cursor& cursorYGet(size_t n) const=0;

		virtual void cursorsRemove()=0;

	protected:
		virtual ~XYPlot()=default;

	private:
		static EventHandler s_default_handler;
	};

#endif

