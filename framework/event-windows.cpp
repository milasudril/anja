#ifdef __WAND__
target[name[event.o] type[object] platform[;Windows]]
#endif

#include "event.h"
#include <windows.h>

class EventWindows:public Event
	{
	public:
		EventWindows();
		~EventWindows();

		void set()
			{
			SetEvent(m_event);
			}

		void wait()
			{
			WaitForSingleObject(m_event,INFINITE);
			}

		void destroy();

	private:
		HANDLE m_event;
	};

Event* Event::create()
	{return new EventWindows();}

void EventWindows::destroy()
	{delete this;}

EventWindows::EventWindows()
	{
	m_event=CreateEvent(NULL,FALSE,FALSE,NULL);
	}

EventWindows::~EventWindows()
	{
	CloseHandle(m_event);
	}
