#ifdef __WAND__
target[name[wavefilereader.o] type[object] dependency[sndfile;external]]
#endif

#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "framework/array_simple.h"
#include <sndfile.h>
#include <cstring>

class WavefileReaderSndfile:public WavefileReader
	{
	public:
		WavefileReaderSndfile(const char* path,WavefileInfo& info);
		~WavefileReaderSndfile();
		unsigned int dataRead(float* buffer, unsigned int n_frames);

	private:
		SF_INFO m_info;
		SNDFILE* m_handle;
	//
	};

WavefileReader* WavefileReader::instanceCreate(const char* path,WavefileInfo& info)
	{
	return new WavefileReaderSndfile(path,info);
	}

void WavefileReader::destroy(WavefileReader* reader)
	{
	delete dynamic_cast<WavefileReader*>(reader);
	}

WavefileReaderSndfile::WavefileReaderSndfile(const char* path,WavefileInfo& info)
	{
	memset(&m_info,0,sizeof(m_info));
	m_handle=sf_open(path,SFM_READ,&m_info);
	if(m_handle==NULL)
		{throw "Could not open file";}
	info.fs=m_info.samplerate;
	info.n_frames=m_info.frames;
	info.n_channels=m_info.channels;
	}

WavefileReaderSndfile::~WavefileReaderSndfile()
	{
	sf_close(m_handle);
	}

unsigned int WavefileReaderSndfile::dataRead(float* buffer, unsigned int n_frames)
	{
	auto n_channels=m_info.channels;
	ArraySimple<float> buffer_tmp(n_frames*n_channels);
	auto N=sf_read_float(m_handle,buffer_tmp.begin(),buffer_tmp.length());
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
