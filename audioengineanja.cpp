#ifdef __WAND__
target[name[audioengineanja.o] type[object]]
#endif

#include "audioengineanja.h"
#include "wavetable.h"
#include "clock.h"
#include "units.h"
#include "playbackrange.h"
#include "midiconstants/controlcodes.h"
#include "midiconstants/statuscodes.h"

#include <cstring>

AudioEngineAnja::AudioEngineAnja(Wavetable& waveforms):
	r_waveforms(&waveforms),m_event_queue(32),m_voice_current(0)
	,m_source_buffers(32),r_source_buffers(waveforms.length())
	,m_voice_channels(m_source_buffers.length())
	{
	m_event_next={0,{MIDIConstants::StatusCodes::INVALID,0,0,0},0.0f};
	}

AudioEngineAnja::~AudioEngineAnja()
	{}

void AudioEngineAnja::onActivate(AudioConnection& source)
	{
	source.audioPortOutputAdd("Audio out");
	m_sample_rate=source.sampleRateGet();
	m_now=0;
	}

void AudioEngineAnja::onDeactivate(AudioConnection& source)
	{}


void AudioEngineAnja::buffersAllocate(AudioConnection& source,unsigned int n_frames)
	{
	m_voice_channels=ArraySimple<float>(n_frames*m_source_buffers.length());
	}


void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,uint8_t value_1) noexcept
	{
	QueueElement tmp;
	tmp.event=
		{
		 secondsToFrames(clockGet(),m_sample_rate)-m_time_start
		,{status,value_0,value_1,0}
		,0.0f
		};
	m_event_queue.push_back(tmp.vector);
	}

void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,float value_1) noexcept
	{
	QueueElement tmp;
	tmp.event=
		{
		 secondsToFrames(clockGet(),m_sample_rate)-m_time_start
		,{status,value_0,0,Event::VALUE_1_FLOAT}
		,value_1
		};
	m_event_queue.push_back(tmp.vector);
	}

void AudioEngineAnja::eventProcess(const AudioEngineAnja::Event& event
	,unsigned int time_offset)
	{
	switch(event.status_word[0]&0xf0)
		{
		case MIDIConstants::StatusCodes::NOTE_ON:
			{
			auto voice=m_voice_current;
			auto slot=event.status_word[1];

			auto& waveform=(*r_waveforms)[slot];

		//	If user tries to load a new waveform in slot while it is playing
		//	a SIGSEGV will occur. Therefore, the waveform object needs to be
		//	marked as locked before we continue.
			waveform.flagsSet(Waveform::LOCKED);
			m_source_buffers[voice].waveformSet(m_randgen,waveform,time_offset);
			r_source_buffers[slot]=voice;
			m_voice_current=(voice+1)%m_source_buffers.length();
			}
			break;

		case MIDIConstants::StatusCodes::NOTE_OFF:
			{
		//	The replace lock cannot be released here, due to the possible
		//	sustain flag. Instead, it has to be done in audioProcess, when
		//	the rendering loop detects that the clip should not continue.
			auto slot=event.status_word[1];
			auto& playback_buffer=m_source_buffers[ r_source_buffers[slot] ];
			if(playback_buffer.flagsGet()&Waveform::SUSTAIN)
				{playback_buffer.flagsUnset(Waveform::LOOP);}
			else
				{playback_buffer.stop();}
			}
			break;
		}
	}

void AudioEngineAnja::audioProcess(AudioConnection& source,unsigned int n_frames) noexcept
	{
	auto& queue=m_event_queue;
	Event event_next=m_event_next;
	auto now=m_now;
	if(now==0)
		{m_time_start=secondsToFrames(clockGet(),m_sample_rate);}
	const auto now_in=now;
	const auto n_frames_in=n_frames;

//	Fetch events
	while(n_frames!=0)
		{
		if(now>=event_next.delay) // If next event has passed
			{
			eventProcess(event_next,now-now_in);
			event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;

			while(!queue.empty())
				{
				QueueElement tmp;
				tmp.vector=queue.pop_front();
				event_next=tmp.event;
				if(now>=event_next.delay)
					{
					eventProcess(event_next,now-now_in);
					event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;
					}
				else
					{break;}
				}
			}
		++now;
		--n_frames;
		}

	auto buffer_out=source.audioBufferOutputGet(0,n_frames_in);
	memset(buffer_out,0,n_frames_in*sizeof(float));

//	Render voices
		{
		auto src_begin=m_source_buffers.begin();
		auto src_current=src_begin;
		auto src_end=m_source_buffers.end();
		auto ptr_voice=m_voice_channels.begin();
		while(src_current!=src_end)
			{
			if(src_current->valid())
				{
				auto N=src_current->outputBufferGenerate(ptr_voice,n_frames_in);
				auto ptr_buffer_out=buffer_out;
				auto ptr_buffer_in=ptr_voice;
				while(N!=0)
					{
					*ptr_buffer_out+=(0.25f) * (*ptr_buffer_in);
					--N;
					++ptr_buffer_in;
					++ptr_buffer_out;
					}
				}
			else
				{
				m_voice_current=src_current-src_begin;
			//	FIXME Reset LOCKED flag for corresponding waveform.
				}
			ptr_voice+=n_frames_in;
			++src_current;
			}
		}
	m_now=now;
	m_event_next=event_next;
	}
