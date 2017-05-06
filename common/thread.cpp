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

static void* thread_entry(void* thread)
	{
	reinterpret_cast<ThreadBase*>(thread)->run();
	return nullptr;
	}

ThreadBase::ThreadBase()
	{
	static_assert(sizeof(m_handle)>=sizeof(pthread_t),"Handle type is too small");
	}

void ThreadBase::start()
	{
	pthread_create(reinterpret_cast<pthread_t*>(&m_handle)
		,NULL,thread_entry,this);
	}

ThreadBase::~ThreadBase() noexcept
	{}

void ThreadBase::synchronize() noexcept
	{pthread_join(m_handle,NULL);}