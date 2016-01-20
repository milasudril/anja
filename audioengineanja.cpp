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
#include "midiconstants/controlcodes.h"

#include <cstring>

AudioEngineAnja::AudioEngineAnja(Wavetable& waveforms):
	r_waveforms(&waveforms),m_event_queue(32),m_voice_current(0)
	,m_source_buffers(32),r_source_buffers(waveforms.length())
	,m_buffer_temp(1),m_buffers_out(16)
	{
	m_event_next={0,{MIDIConstants::StatusCodes::INVALID,0,0,0},0.0f};
		{
		auto ptr_channel=m_channels.begin();
		while(ptr_channel!=m_channels.end())
			{
			*ptr_channel=1.0f;
			++ptr_channel;
			}
		}
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
	m_buffer_temp=ArraySimple<float>(n_frames);
	m_buffers_out=ArraySimple<float>(n_frames*m_channels.length());
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

void AudioEngineAnja::eventControlProcess(const AudioEngineAnja::Event& event)
	{
	switch(event.status_word[1])
		{
		case MIDIConstants::ControlCodes::CHANNEL_VOLUME:
			{
			auto value=event.status_word[3]==Event::VALUE_1_FLOAT?
				event.value : float(event.status_word[2])/127.0f;
			auto channel=event.status_word[0]&0xf;
			m_channels[channel]=value;
			}
			break;
		}
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

		//	When note is stopped in sustain mode, ensure that the LOOP flag is
		//	is disabled to prevent infinite sound
			if(playback_buffer.flagsGet()&Waveform::SUSTAIN)
				{playback_buffer.flagsUnset(Waveform::LOOP);}
			else
				{playback_buffer.stop();}
			}
			break;

		case MIDIConstants::StatusCodes::CONTROLLER:
			eventControlProcess(event);
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
		//	Set the status code to invalid so it does not do anything more
			event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;

		//	Check if there are any more events
			while(!queue.empty())
				{
				QueueElement tmp;
				tmp.vector=queue.pop_front();
				event_next=tmp.event;
				if(now>=event_next.delay) //Process next event now
					{
					eventProcess(event_next,now-now_in);
					event_next.status_word[0]=MIDIConstants::StatusCodes::INVALID;
					}
				else //	It is not time for current event yet. Exit the loop
					{break;}
				}
			}
		++now;
		--n_frames;
		}

//	Render voices
		{
		memset(m_buffers_out.begin(),0,m_buffers_out.length()*sizeof(float));
		auto src_begin=m_source_buffers.begin();
		auto src_current=src_begin;
		auto src_end=m_source_buffers.end();
		while(src_current!=src_end)
			{
			if(src_current->valid())
				{
				auto ptr_voice=m_buffer_temp.begin();
				auto N=src_current->outputBufferGenerate(ptr_voice,n_frames_in);
				auto channel=src_current->channelGet();
				auto gain_channel=m_channels[channel];
				auto ptr_buffer_out=m_buffers_out.begin()+n_frames_in*channel;
				auto ptr_buffer_in=ptr_voice;
				while(N!=0)
					{
					*ptr_buffer_out+=gain_channel * (*ptr_buffer_in);
					--N;
					++ptr_buffer_in;
					++ptr_buffer_out;
					}
				if(src_current->done())
					{
					src_current->release();
					m_voice_current=src_current-src_begin;
					}
				}
			++src_current;
			}
		}

	//	Mix channels
		{
		auto buffer_out=source.audioBufferOutputGet(0,n_frames_in);
		memset(buffer_out,0,n_frames_in*sizeof(float));
		auto N_ch=m_channels.length();
		auto ptr_in=m_buffers_out.begin();
		while(N_ch!=0)
			{
			auto ptr_out=buffer_out;
			auto N=n_frames_in;
			while(N!=0)
				{
				*ptr_out+=*ptr_in;
				++ptr_in;
				++ptr_out;
				--N;
				}
			--N_ch;
			}
		}
	m_now=now;
	m_event_next=event_next;
	}
