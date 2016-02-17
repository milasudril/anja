#ifdef __WAND__
target[name[wavefilewriter.o] type[object] dependency[sndfile;external]]
#endif

#include "wavefilewriter.h"
#include "wavefileinfo.h"
#include "framework/array_simple.h"
#include <sndfile.h>
#include <cstring>

class WavefileWriterSndfile:public WavefileWriter
	{
	public:
		WavefileWriterSndfile(const char* path,const WavefileInfo& info);
		~WavefileWriterSndfile();
		unsigned int dataWrite(const float* buffer, unsigned int n_frames);

	private:
		SF_INFO m_info;
		SNDFILE* m_handle;
	};

WavefileWriter* WavefileWriter::instanceCreate(const char* path,const WavefileInfo& info)
	{
	return new WavefileWriterSndfile(path,info);
	}

void WavefileWriter::destroy(WavefileWriter* reader)
	{
	delete dynamic_cast<WavefileWriter*>(reader);
	}

WavefileWriterSndfile::WavefileWriterSndfile(const char* path,const WavefileInfo& info)
	{
	memset(&m_info,0,sizeof(m_info));
	m_info.samplerate=info.fs;
	m_info.channels=info.n_channels;
	m_info.format=SF_FORMAT_WAVEX | SF_FORMAT_FLOAT;
	m_handle=sf_open(path,SFM_WRITE,&m_info);
	if(m_handle==NULL)
		{throw "Could not open file";}
	}

WavefileWriterSndfile::~WavefileWriterSndfile()
	{
	sf_close(m_handle);
	}

unsigned int WavefileWriterSndfile::dataWrite(const float* buffer, unsigned int n_frames)
	{
	return sf_write_float(m_handle,buffer,n_frames);
	}
