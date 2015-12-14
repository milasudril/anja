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
				virtual void mouseMove(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void mouseDown(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void mouseUp(const Curve::Point& x,keymask_t key_mask)
					{}

				virtual void keyDown(uint8_t scancode)
					{}

				virtual void keyUp(uint8_t scancode)
					{}
			};

		static XYPlot* create(GuiContainer& parent,EventHandler& handler)
			{return new XYPlot(parent,handler);}

		void destroy()
			{delete this;}

		void update();

		void curveAdd(const Curve& curve);

		void domainSet(const Curve::Domain& domain);

		void backgroundSet(bool light);

	private:
		XYPlot(GuiContainer& parent,EventHandler& handler);
		~XYPlot();

		class Impl;
		Impl* m_impl;
	};

#endif

