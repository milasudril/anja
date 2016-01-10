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
	,m_playback_buffers(32)
	{
	size_t N=m_event_queue.length();
	while(N!=0)
		{
		m_event_queue.push_back({0,{MIDIConstants::StatusCodes::INVALID,0,0,0},0.0f});
		--N;
		}
	m_event_next={0,{MIDIConstants::StatusCodes::INVALID,0,0,0},0.0f};
	}

AudioEngineAnja::~AudioEngineAnja()
	{}

void AudioEngineAnja::onActivate(AudioConnection& source)
	{
	source.audioPortOutputAdd("Audio out");
	m_sample_rate=source.sampleRateGet();
	m_time_start=clockGet();
	m_now=m_time_start;
	}

void AudioEngineAnja::onDeactivate(AudioConnection& source)
	{}


void AudioEngineAnja::buffersAllocate(AudioConnection& source,unsigned int n_frames)
	{}


void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,uint8_t value_1) noexcept
	{
	m_event_queue.push_back(
		{
		 secondsToFrames(clockGet()-m_time_start,m_sample_rate)
		,{status,value_0,value_1,0}
		,0.0f
		});
	}

void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,float value_1) noexcept
	{
	m_event_queue.push_back(
		{
		 secondsToFrames(clockGet()-m_time_start,m_sample_rate)
		,{status,value_0,0,Event::VALUE_1_FLOAT}
		,value_1
		});
	}

void AudioEngineAnja::eventProcess(const AudioEngineAnja::Event& event
	,unsigned int time_offset)
	{
	switch(event.status_word[0]&0xf0)
		{
		case MIDIConstants::StatusCodes::NOTE_ON:
		//	TODO Add polyphony
			m_playback_buffers[0]={(*r_waveforms)[event.status_word[1]],time_offset};
			break;

		case MIDIConstants::StatusCodes::NOTE_OFF:
			printf("Shut up\n");
			break;
		}
	}

void AudioEngineAnja::audioProcess(AudioConnection& source,unsigned int n_frames) noexcept
	{
	auto& queue=m_event_queue;
	Event event_next=m_event_next;
	auto now=m_now;
	auto now_in=now;
	auto n_frames_in=n_frames;

	while(n_frames!=0)
		{
		if(now>=event_next.delay)
			{
			eventProcess(event_next,now-now_in);
			event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;
			}

		while(!queue.empty())
			{
			event_next=queue.pop_front();
			if(now>=event_next.delay)
				{
				eventProcess(event_next,now-now_in);
				event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;
				}
			else
				{break;}
			}

		++now;
		--n_frames;
		}

	auto buffer=source.audioBufferOutputGet(0,n_frames_in);
	memset(buffer,0,n_frames_in*sizeof(float));
	if(m_playback_buffers[0].valid())
		{
		m_playback_buffers[0].outputBufferGenerate(buffer,n_frames_in);
		}
	m_now=now;
	m_event_next=event_next;
	}
