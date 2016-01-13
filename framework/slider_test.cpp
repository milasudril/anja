#ifdef __WAND__
target[name[slider_test] type[application] platform[;GNU/Linux]]
#endif

#include "slider.h"
#include "window.h"
#include "boxhorizontal.h"

int main()
	{
	auto event_loop=EventLoop::create(0);
	auto mainwin=Window::create(*event_loop);
	auto box=BoxHorizontal::create(*mainwin);
	Slider* sliders[16];
	for(size_t k=0;k<16;++k)
		{sliders[k]=Slider::create(*box,"Level",0,0);}

	return 0;
	}
