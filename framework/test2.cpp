#ifdef __WAND__
target[name[test2] type[application] platform[;GNU/Linux]]
#endif

#include "slider.h"
#include "mainloop.h"
#include "guicontainer.h"

#include <cstdio>
#include <cstdlib>

class SliderEvents:public Slider::EventHandler
	{
	public:
		const char* textGet(double value)
			{
			sprintf(m_buffer,"%.3f",value);
			return m_buffer;
			}

		double valueGet(const char* buffer)
			{
			return atof(buffer);
			}

	private:
		char m_buffer[32];
	};

int main(int argc,char** argv)
	{
	init(argc,argv);
	GuiContainer::EventHandler handler;
	auto mainwin=GuiContainer::create(handler);
	SliderEvents se;
	auto slider=Slider::create(*mainwin,se);
	slider->valueSet(0.5);
	run();
	return 0;
	}
