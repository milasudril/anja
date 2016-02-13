#ifdef __WAND__
target[name[thread.o] type[object] platform[;GNU/Linux] dependency[pthread;external]]
#endif

#include "thread.h"
#include <pthread.h>
#include <cstdint>

class ThreadLinux:public Thread
	{
	public:
		ThreadLinux(Task& task);

		~ThreadLinux();

		void destroy() noexcept
			{delete this;}

		unsigned int exitStatusGet() const noexcept
			{return m_exit_status;}

	private:
		Task& r_task;
		pthread_t m_thread;
		unsigned int m_exit_status;
		static void* thread_startup(void* task);
	};

void* ThreadLinux::thread_startup(void* task)
	{
	try
		{
		Task* t=reinterpret_cast<Task*>(task);
		return reinterpret_cast<void*>(t->run());
		}
	catch(...)
		{return reinterpret_cast<void*>(-1);}
	}

Thread* Thread::create(Task& task)
	{return new ThreadLinux(task);}

ThreadLinux::ThreadLinux(Task& task):r_task(task)
	{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&m_thread,&attr,thread_startup,&r_task);
	pthread_attr_destroy(&attr);
	}

ThreadLinux::~ThreadLinux()
	{
	void* exit_status;
	pthread_join(m_thread,&exit_status);
	m_exit_status=reinterpret_cast<intptr_t>(exit_status);
	r_task.onCompleted(*this);
	}

