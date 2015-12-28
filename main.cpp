#ifdef __WAND__
target[name[anja] type[application]]
#endif


#include "session.h"
#include "sessionview.h"
#include "framework/window.h"

int main()
	{
	try
		{
		Session session("testbank/testbank.txt");
		auto event_loop=EventLoop::create();
		auto mainwin=Window::create(*event_loop);

		ArrayDynamicShort<char> title("Anja - ");

		title.truncate().append(session.titleGet());

		mainwin->titleSet( title.begin() );
		SessionView::create(*mainwin,session);
		}
	catch(const char* msg)
		{printf("Error: %s\n",msg);}
	return 0;
	}
