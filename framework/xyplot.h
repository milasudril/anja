#ifdef __WAND__
target[name[xyplot.h] type[include]]
dependency[xyplot.o]
#endif

#ifndef XYPLOT_H
#define XYPLOT_H

#include "keymask.h"
#include "curve.h"
#include <cstdint>
#include <cstddef>

class GuiContainer;

class XYPlot
	{
	public:
		class EventHandler
			{
			public:
				virtual void onMouseMove(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void onMouseDown(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void onMouseUp(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void onKeyDown(uint8_t scancode)
					{}

				virtual void onKeyUp(uint8_t scancode)
					{}
			};

		static XYPlot* create(GuiContainer& parent,EventHandler& handler);

		virtual void destroy()=0;
		virtual void update()=0;
		virtual void curveAdd(const Curve& curve)=0;
		virtual void domainSet(const Curve::Domain& domain)=0;
		virtual void backgroundSet(bool light)=0;

	protected:
		virtual ~XYPlot()=default;

	private:
		static EventHandler s_default_handler;
	};

#endif

