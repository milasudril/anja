#ifdef __WAND__
target[name[xyplot_test] type[application] platform[;GNU/Linux]]
#endif

#include "xyplot.h"
#include "curve.h"
#include "window.h"
#include "color.h"

#include <cmath>
#include <cstdio>

int main(int argc,char** argv)
	{
	constexpr size_t N=1024;
	Curve::Point data_0[N+1];
	Curve::Point data_1[N+1];
	for(size_t k=0;k<=N;++k)
		{
		auto x=2*acos(-1)*(double(k)/N - 0.5);
		data_0[k]={x,sin(x)};
		data_1[k]={x,cos(x)};
		}

	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	auto plotwindow=XYPlot::create(*mainwin);
	plotwindow->curveAdd({data_0,N+1,COLORS[ColorID::YELLOW]});
	plotwindow->curveAdd({data_1,N+1,COLORS[ColorID::BLUE]});
	return 0;
	}
