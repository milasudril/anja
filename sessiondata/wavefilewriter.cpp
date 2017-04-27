//@	 {"targets":[{"name":"wavefilewriter.o","type":"object","pkgconfig_libs":"sndfile"}]}

#include "wavefilewriter.hpp"
#include "wavefileinfo.hpp"
#include "../common/arraysimple.hpp"
#include <sndfile.h>
#include <cstring>

using namespace Anja;

class WavefileWriter::Impl
	{
	public:
		Impl(const char* path,const WavefileInfo& info);
		~Impl();
		unsigned int dataWrite(const float* buffer, unsigned int n_frames);

	private:
		SF_INFO m_info;
		SNDFILE* m_handle;
	};

WavefileWriter::WavefileWriter(const char* path,WavefileInfo& info)
	{m_impl.reset(new Impl(path,info));}

WavefileWriter::~WavefileWriter()
	{}

unsigned int WavefileWriter::dataWrite(const float* buffer, unsigned int n_frames)
	{return m_impl->dataWrite(buffer,n_frames);}

WavefileWriter::Impl::Impl(const char* path,const WavefileInfo& info)
	{
	memset(&m_info,0,sizeof(m_info));
	m_info.samplerate=info.fs;
	m_info.channels=info.n_channels;
	m_info.format=SF_FORMAT_WAVEX | SF_FORMAT_FLOAT;
	m_handle=sf_open(path,SFM_WRITE,&m_info);
	if(m_handle==NULL)
		{throw "Could not open file";}
	}

WavefileWriter::Impl::~Impl()
	{
	sf_close(m_handle);
	}

unsigned int WavefileWriter::Impl::dataWrite(const float* buffer, unsigned int n_frames)
	{
	return sf_write_float(m_handle,buffer,n_frames);
	}
