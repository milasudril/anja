#ifdef __WAND__
target[name[waveform.o] type[object]]
#endif

#include "waveform.h"
#include "optionstring.h"
#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "framework/array_simple.h"

const char* Waveform::FLAG_NAMES[]=
	{"Loop","Sustain","Readonly","Set gain on loop",nullptr};

void Waveform::fileLoad(const char* filename)
	{
//	Stop loading new data into this slot if it is already in use by an audio
//	thread.
	if(flagsGet() & Waveform::LOCKED)
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
		m_state_flags&=~DIRTY;
		return;
		}

//	Try to load the file
	WavefileInfo info;
	auto reader=WavefileReader::create(filename,info);
	clear();
	sampleRateSet(info.fs);
	capacitySet(info.n_frames);
	const uint32_t buffer_size=1024;
	ArraySimple<float> buffer_tmp(buffer_size);
	uint32_t n_read=0;
	do
		{
		n_read=reader->dataRead(buffer_tmp.begin(),buffer_size);
		append(buffer_tmp.begin(),n_read);
		}
	while(n_read==buffer_size);
	offsetsReset();

//	When a wavefile is loaded, we do not want to overwrite data by accident.
	flagsSet(Waveform::READONLY);

	dirtyClear();
	}
