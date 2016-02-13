#ifdef __WAND__
target
	[
	name[eventloop.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "eventloop.h"
#include <windows.h>

class EventLoopWindows:public EventLoop
	{
	public:
		EventLoopWindows(bool background_light);
		void run();

	private:
		static bool s_init;
	};

bool EventLoopWindows::s_init=0;


EventLoopWindows::EventLoopWindows(bool background_light)
	{
	if(!s_init)
		{
		WNDCLASSEXA windowClass;
		memset(&windowClass,0,sizeof(windowClass));
		windowClass.cbSize=sizeof(windowClass);
		windowClass.style=CS_DBLCLKS;
		windowClass.lpfnWndProc=DefWindowProcA;
		windowClass.hInstance=GetModuleHandle(NULL);
		windowClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		windowClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE + 1);
		windowClass.lpszClassName="AnjaWindow";
		RegisterClassExA(&windowClass);

	//	TODO InitCommonControlsEx
		s_init=1;
		}
	}

void EventLoopWindows::run()
	{
	MSG message;
	while(windowsLeft()!=0)
		{
		if(!GetMessageA(&message,NULL,0,0))
			{break;}
		TranslateMessage(&message);
		DispatchMessageA(&message);
		}
	delete this;
	}

EventLoop* EventLoop::instanceCreate(bool background_light)
	{
	return new EventLoopWindows(background_light);
	}
