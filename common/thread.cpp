//@	{
//@	 "targets":
//@		[{
//@		 "name":"thread.o","type":"object"
//@		,"dependencies":[{"ref":"pthread","rel":"external"}]
//@		}]
//@	}

#include "thread.hpp"
#include <pthread.h>

using namespace Anja;

void* Thread::thread_entry_system(void* thread)
	{
	auto self=reinterpret_cast<Thread*>(thread);
	self->m_cb(self->r_cb_obj);
	return nullptr;
	}

void Thread::run()
	{
	static_assert(sizeof(m_handle)>=sizeof(pthread_t),"Handle type is too small");
	pthread_create(reinterpret_cast<pthread_t*>(&m_handle)
		,NULL,thread_entry_system,this);
	}

Thread::~Thread()
	{pthread_join(m_handle,NULL);}
