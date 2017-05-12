//@	 {"targets":[{"name":"wavefilereader.o","type":"object","pkgconfig_libs":["sndfile"]}]}


#include "wavefilereader.hpp"
#include "wavefileinfo.hpp"
#include "../common/arraysimple.hpp"
#include "../common/pathutils.hpp"

#include <sndfile.h>
#include <cstring>

using namespace Anja;

class WavefileReader::Impl
	{
	public:
		Impl(const char* path,WavefileInfo& info);
		~Impl();
		unsigned int dataRead(float* buffer, unsigned int n_frames);

		static bool check(const char* path,WavefileInfo& info);

	private:
		SF_INFO m_info;
		SNDFILE* m_handle;
	};

WavefileReader::WavefileReader(const char* path,WavefileInfo& info)
	{m_impl.reset(new Impl(path,info));}

WavefileReader::~WavefileReader()
	{}

unsigned int WavefileReader::dataRead(float* buffer, unsigned int n_frames)
	{return m_impl->dataRead(buffer,n_frames);}



WavefileReader::Impl::Impl(const char* path,WavefileInfo& info)
	{
	memset(&m_info,0,sizeof(m_info));
	m_handle=sf_open(path,SFM_READ,&m_info);
	if(m_handle==NULL)
		{throw "Could not open file";}
	info.fs=m_info.samplerate;
	info.n_frames=m_info.frames;
	info.n_channels=m_info.channels;
	}

WavefileReader::Impl::~Impl()
	{
	sf_close(m_handle);
	}

unsigned int WavefileReader::Impl::dataRead(float* buffer, unsigned int n_frames)
	{
	auto n_channels=m_info.channels;
	ArraySimple<float> buffer_tmp(n_frames*n_channels);
	auto N=sf_read_float(m_handle,buffer_tmp.begin(),buffer_tmp.length())
		/n_channels;
	auto N_ret=N;
	auto ptr_buffer=buffer_tmp.begin();
	while(N!=0)
		{
		*buffer=*ptr_buffer;
		ptr_buffer+=n_channels;
		++buffer;
		--N;
		}
	return N_ret;
	}

bool WavefileReader::check(const char* path,WavefileInfo& info)
	{
	return fileIs(path) && WavefileReader::Impl::check(path,info);
	}

bool WavefileReader::Impl::check(const char* path,WavefileInfo& info)
	{
	SF_INFO sfinfo;
	auto handle=sf_open(path,SFM_READ,&sfinfo);
	if(handle==NULL)
		{return 0;}
	sf_close(handle);
	info.fs=sfinfo.samplerate;
	info.n_frames=sfinfo.frames;
	info.n_channels=sfinfo.channels;
	return 1;
	}
