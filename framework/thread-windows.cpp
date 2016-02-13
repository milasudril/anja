#ifdef __WAND__
target[name[thread.o] type[object] platform[;Windows]]
#endif

#include "thread.h"
#include <windows.h>
#include <cstdint>

class ThreadWindows:public Thread
	{
	public:
		ThreadWindows(Task& task);

		~ThreadWindows();

		void destroy() noexcept
			{delete this;}

		unsigned int exitStatusGet() const noexcept
			{return m_exit_status;}

	private:
		Task& r_task;
		HANDLE m_handle;
		DWORD m_exit_status;
		static DWORD WINAPI thread_startup(void* task);
	};

DWORD WINAPI ThreadWindows::thread_startup(void* task)
	{
	try
		{
		Task* t=reinterpret_cast<Task*>(task);
		return t->run();
		}
	catch(...)
		{return -1;}
	}

Thread* Thread::create(Task& task)
	{return new ThreadWindows(task);}

ThreadWindows::ThreadWindows(Task& task):r_task(task)
	{
	DWORD tid;
	m_handle=CreateThread(NULL,0,thread_startup,&r_task,0,&tid);
	}

ThreadWindows::~ThreadWindows()
	{
	WaitForSingleObject(m_handle,INFINITE);
	GetExitCodeThread(m_handle,&m_exit_status);
	r_task.onCompleted(*this);
	}

