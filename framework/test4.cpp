#ifdef __WAND__
target[name[test4] type[application] platform[;GNU/Linux]]
#endif

#include "xyplot.h"
#include "curve.h"
#include "mainloop.h"
#include "guicontainer.h"
#include "color.h"

#include <cmath>
#include <cstdio>

class EventHandlerPlot:public XYPlot::EventHandler
	{
	public:
		EventHandlerPlot():r_view(nullptr),m_state(0)
			{}

		void viewSet(XYPlot& view)
			{r_view=&view;}

		void mouseUp(const Curve::Point& point,keymask_t keymask)
			{
			if(r_view!=nullptr)
				{
				r_view->backgroundSet(m_state);
				m_state=!m_state;
				}
			printf("%.15g %.15g\n",point.x,point.y);
			}
	private:
		XYPlot* r_view;
		bool m_state;
	};


int main(int argc,char** argv)
	{
	init(argc,argv);
	constexpr size_t N=1024;
	Curve::Point data_0[N+1];
	Curve::Point data_1[N+1];
	for(size_t k=0;k<=N;++k)
		{
		auto x=2*acos(-1)*(double(k)/N - 0.5);
		data_0[k]={x,sin(x)};
		data_1[k]={x,cos(x)};
		}

	GuiContainer::EventHandler we;
	auto mainwin=GuiContainer::create(we);
	EventHandlerPlot pe;
	auto plotwindow=XYPlot::create(*mainwin,pe);
	plotwindow->curveAdd({data_0,N+1,COLORS[ColorID::YELLOW]});
	plotwindow->curveAdd({data_1,N+1,COLORS[ColorID::BLUE]});
	plotwindow->backgroundSet(1);
	pe.viewSet(*plotwindow);
//	plotwindow->domainSet({{0,-1},{acos(-1),1}});
	run();

	return 0;
	}
