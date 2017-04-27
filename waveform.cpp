#ifdef __WAND__
target[name[waveform.o] type[object]]
#endif

#include "waveform.h"
#include "optionstring.h"
#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "wavefilewriter.h"
#include "framework/array_simple.h"

static constexpr unsigned int BUFFER_SIZE=1024;

const char* Waveform::FLAG_NAMES[]=
	{"Loop","Sustain","Readonly","Set gain on loop",nullptr};

void Waveform::fileLoad(const char* filename)
	{
//	Stop loading new data into this slot if it is already in use by an audio
//	thread.
	if(flagsGet() & (PLAYBACK_RUNNING|RECORD_RUNNING) )
		{
		throw "The waveform loaded in the current slot is currently in use. "
			"Please wait for the waveform to be unlocked, or choose another "
			"slot.";
		}

//	If filename is empty, clear the slot
	if(*filename=='\0')
		{
		clear();
		float vals[2]={1e-7f,1e-7f};
		append(vals,2);
		valuesInit();
		sampleRateSet(1000.0);
		dirtyClear();
		return;
		}

//	Try to load the file
	WavefileInfo info;
	auto reader=WavefileReader::create(filename,info);
	clear();
	sampleRateSet(info.fs);
	capacitySet(info.n_frames);
	ArraySimple<float> buffer_tmp(BUFFER_SIZE);
	uint32_t n_read=0;
	do
		{
		n_read=reader->dataRead(buffer_tmp.begin(),BUFFER_SIZE);
		append(buffer_tmp.begin(),n_read);
		}
	while(n_read==BUFFER_SIZE);
	offsetsReset();

//	When a wavefile is loaded, we do not want to overwrite data by accident.
	flagsSet(Waveform::READONLY);

	dirtyClear();
	}



void Waveform::fileSave(const char* filename)
	{
	if(flagsGet() & (PLAYBACK_RUNNING|RECORD_RUNNING))
		{
		throw "The waveform loaded in the current slot is currently in use. "
			"Please wait for the waveform to be unlocked.";
		}

	WavefileInfo info{lengthFull(),sampleRateGet(),1};

	auto writer=WavefileWriter::create(filename,info);
	auto ptr=beginFull();
	auto n=lengthFull();
	while(n!=0)
		{
		auto n_written=writer->dataWrite(ptr,std::min(BUFFER_SIZE,n));
		n-=n_written;
		ptr+=n_written;
		}
	m_flags&=~RECORDED;
	}
