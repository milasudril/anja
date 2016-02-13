#ifdef __WAND__
target[name[recordbuffers.o] type[object]]
#endif

#include "recordbuffers.h"
#include "framework/event.h"

#include <cstring>

RecordBuffers::RecordBuffers(unsigned int n_frames):m_data({n_frames,n_frames})
	,m_buffer_current(0),m_n_written(0)
	{
	m_ready=Event::create();
	}

RecordBuffers::~RecordBuffers()
	{
	m_ready->destroy();
	}

const ArraySimple<float>& RecordBuffers::bufferGet() const
	{
	m_ready->wait();
	return m_data[(m_buffer_current + 1)%2];
	}

void RecordBuffers::readySet() noexcept
	{
	m_buffer_current=(m_buffer_current + 1)%2;
	m_ready->set();
	}

void RecordBuffers::write(const float* buffer,unsigned int N) noexcept
	{
	auto ptr_out=m_data[m_buffer_current].begin();
	auto length=m_data[m_buffer_current].length();
	auto n_written=m_n_written;
	while(N!=0)
		{
		if(n_written==length)
			{
			n_written=0;
			m_buffer_current=(m_buffer_current + 1)%2;
			m_ready->set();
			ptr_out=m_data[m_buffer_current].begin();
			length=m_data[m_buffer_current].length();
		//	FIXME: Return the correct length from buffer get;
			memset(ptr_out,length,sizeof(float));
			}
		auto n=std::min(size_t(N),length-n_written);
		memcpy(ptr_out,buffer,n*sizeof(float));
		N-=n;
		ptr_out+=n;
		n_written+=n;
		}
	m_n_written=n_written;
	}
