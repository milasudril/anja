#ifdef __WAND__
target[name[event.o] type[object] platform[;GNU/Linux] dependency[pthread;external]]
#endif

#include "event.h"
#include <pthread.h>

class EventLinux:public Event
	{
	public:
		EventLinux();
		~EventLinux();

		void set();

		void wait();

		void destroy();

		void reset();

	private:
		pthread_cond_t m_cond;
		pthread_mutex_t m_mutex;
		volatile bool m_signaled;
	};

Event* Event::create()
	{return new EventLinux();}

void EventLinux::destroy()
	{delete this;}

EventLinux::EventLinux()
	{
	pthread_cond_init(&m_cond,NULL);
	pthread_mutex_init(&m_mutex,NULL);
	m_signaled=0;
	}

EventLinux::~EventLinux()
	{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
	}

void EventLinux::set()
	{
	pthread_mutex_lock(&m_mutex);
	pthread_cond_signal(&m_cond);
	m_signaled=1;
	pthread_mutex_unlock(&m_mutex);
	}

void EventLinux::reset()
	{
	m_signaled=0;
	}

void EventLinux::wait()
	{
	pthread_mutex_lock(&m_mutex);
	while(!m_signaled)
		{pthread_cond_wait(&m_cond,&m_mutex);}
	m_signaled=0;
	pthread_mutex_unlock(&m_mutex);
	}
