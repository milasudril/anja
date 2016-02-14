#ifdef __WAND__
target[name[recordbuffers.o] type[object]]
#endif

#include "recordbuffers.h"
#include "framework/event.h"

#include <cstring>

RecordBuffers::RecordBuffers(unsigned int n_frames):
	m_data({{n_frames,0},{n_frames,0}})
	,m_buffer_current(0),m_buffer_current_out(0)
	{
	m_ready=Event::create();
	}

RecordBuffers::~RecordBuffers()
	{
	m_ready->destroy();
	}

const RecordBuffers::WaveformBuffer& RecordBuffers::bufferGet() const
	{
	m_ready->wait();
	return m_data[m_buffer_current_out];
	}

void RecordBuffers::readySet() noexcept
	{
	m_buffer_current_out=m_buffer_current;
	m_ready->set();
	}

void RecordBuffers::write(const float* buffer,unsigned int N) noexcept
	{
	auto buffer_current=m_buffer_current;
	auto ptr_out=m_data[buffer_current].m_data.begin();
	auto length=m_data[buffer_current].m_data.length();
	auto n_written=m_data[buffer_current].m_n_written;
	ptr_out+=n_written;
	while(N!=0)
		{
		if(n_written==length)
			{
		//	We have filled the buffer. Commit results and release the lock.
			m_data[buffer_current].m_n_written=n_written;
			m_buffer_current_out=buffer_current;
			m_ready->set();

		//	Swap buffers and start again
			buffer_current=(buffer_current + 1)%2;
			n_written=0;
			ptr_out=m_data[buffer_current].m_data.begin();
			length=m_data[buffer_current].m_data.length();

		//	Clear current buffer
			memset(ptr_out,length,sizeof(float));
			}
		auto n=std::min(size_t(N),length-n_written);
		memcpy(ptr_out,buffer,n*sizeof(float));
		N-=n;
		ptr_out+=n;
		n_written+=n;
		}
	m_data[buffer_current].m_n_written=n_written;
	m_buffer_current=buffer_current;
	}
