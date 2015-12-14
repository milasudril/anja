#ifdef __WAND__
target[name[anja] type[application]]
#endif

#include "framework/mainloop.h"
#include "framework/guicontainer.h"

int main(int argc, char* argv[])
	{
	init(argc,argv);
	GuiContainer::EventHandler we;
	auto mainwin=GuiContainer::create(we);
	run();
	return 0;
	}
