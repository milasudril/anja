//@	{
//@	"targets":
//@		[{
//@		"name":"readysignal.o","type":"object"
//@		,"dependencies":[{"ref":"pthread","rel":"external"}]
//@		}]
//@	}

#include "readysignal.hpp"
#include <pthread.h>

using namespace Anja;

class ReadySignal::Impl
	{
	public:
		Impl();
		~Impl();

		void set()
			{
			pthread_mutex_lock(&m_mutex);
			pthread_cond_signal(&m_cond);
			m_signaled=1;
			pthread_mutex_unlock(&m_mutex);
			}

		void wait()
			{
			pthread_mutex_lock(&m_mutex);
			while(!m_signaled)
				{pthread_cond_wait(&m_cond,&m_mutex);}
			m_signaled=0;
			pthread_mutex_unlock(&m_mutex);
			}

		void reset()
			{
			m_signaled=0;
			}

	private:
		pthread_cond_t m_cond;
		pthread_mutex_t m_mutex;
		volatile bool m_signaled;
	};

ReadySignal::ReadySignal()
	{m_impl=new Impl;}

ReadySignal::~ReadySignal()
	{delete m_impl;}

void ReadySignal::set() noexcept
	{m_impl->set();}

void ReadySignal::wait() noexcept
	{m_impl->wait();}

void ReadySignal::reset() noexcept
	{m_impl->reset();}


ReadySignal::Impl::Impl()
	{
	pthread_cond_init(&m_cond,NULL);
	pthread_mutex_init(&m_mutex,NULL);
	m_signaled=0;
	}

ReadySignal::Impl::~Impl()
	{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
	}
