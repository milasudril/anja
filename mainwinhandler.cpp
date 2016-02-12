#ifdef __WAND__
target[name[mainwinhandler.o] type[object]]
#endif

#include "mainwinhandler.h"
#include "sessionactions.h"

void MainwinHandler::onClose(Window& source)
	{
	r_actions->askSave();
	source.destroy();
	}
