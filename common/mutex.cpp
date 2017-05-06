//@	{
//@	"targets":
//@		[{
//@		 "dependencies":[{"ref":"pthread","rel":"external"}]
//@		,"name":"mutex.o","type":"object"
//@	    }]
//@	}

#include "mutex.hpp"
#include <pthread.h>

using namespace Anja;

static pthread_mutex_t* mutex(uint8_t* bytes)
	{
	return reinterpret_cast<pthread_mutex_t*>(bytes);
	}

Mutex::Mutex()
	{
	static_assert(sizeof(m_impl)==sizeof(pthread_mutex_t),"Implementation size is wrong");
	pthread_mutex_init(mutex(m_impl.data),NULL);
	}

Mutex::~Mutex()
	{
	pthread_mutex_destroy(mutex(m_impl.data));
	}

void Mutex::lock() noexcept
	{
	pthread_mutex_lock(mutex(m_impl.data));
	}

void Mutex::unlock() noexcept
	{
	pthread_mutex_unlock(mutex(m_impl.data));
	}

bool Mutex::lockTry() noexcept
	{
	return pthread_mutex_trylock(mutex(m_impl.data))==0;
	}
