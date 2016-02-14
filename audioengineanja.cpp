#ifdef __WAND__
target[name[audioengineanja.o] type[object]]
#endif

#include "audioengineanja.h"
#include "wavetable.h"
#include "clock.h"
#include "units.h"
#include "playbackrange.h"
#include "recordbuffers.h"
#include "filterstep.h"
#include "midiconstants/controlcodes.h"
#include "midiconstants/statuscodes.h"
#include "midiconstants/controlcodes.h"
#include "framework/event.h"
#include "framework/minifloat.h"

#include <cstring>

AudioEngineAnja::RecordTask::RecordTask()
	{
	m_ready=::Event::create();
	m_flags=0;
	}

AudioEngineAnja::RecordTask::~RecordTask()
	{
	m_ready->destroy();
	}

AudioEngineAnja::RecordTask& AudioEngineAnja::RecordTask::disable() noexcept
	{
	if(enabled())
		{
		m_flags&=~ENABLED;
	//	NOTE: Synchronization is needed here. Otherwise, the record thread may
	//	not keep up when updating playback offsets.
		printf("I am here: %s %u\n",__FILE__,__LINE__);
		m_ready->wait();
		}
	return *this;
	}

unsigned int AudioEngineAnja::RecordTask::run()
	{
	while(!stopped())
		{
		auto& buffer=r_rec_buffers->bufferGet();
		auto n_written=buffer.m_n_written;

	//	NOTE: Capture the pointer here so we get a local copy of it. The other
	//	thread may set r_waveform to 0 before we get to the append statement.
		auto waveform=r_waveform;
		if(waveform!=nullptr)
			{
			waveform->append(buffer.m_data.begin(),n_written);
		//	NOTE: We should write remaining data to waveform. When done
		//	clear record flag and reset playback offsets.
			if(!enabled())
				{
				waveform->flagsUnset(Waveform::RECORD_RUNNING).offsetsReset();
				r_waveform=nullptr;
				m_ready->set();
				}
			}
		}
	return 0;
	}

AudioEngineAnja::RecordTask& AudioEngineAnja::RecordTask::waveformSet(Waveform& waveform) noexcept
	{
	r_waveform=&waveform;
	return *this;
	}


AudioEngineAnja::AudioEngineAnja(Wavetable& waveforms):
	r_waveforms(&waveforms),m_sample_rate(0),m_event_queue(32)
	,m_voice_current(0)
	,m_source_buffers(32),r_source_buffers(waveforms.length())
	,m_fader_filter_factor(0)
	,m_buffer_temp(1),m_buffers_out(16)
	,m_master_gain_out(1.0),m_master_gain_in(1.0),m_multioutput(0)
	{
	m_ready=::Event::create();
	m_record_complete=::Event::create();
	reset();
	}

AudioEngineAnja::~AudioEngineAnja()
	{
	m_ready->destroy();
	m_record_complete->destroy();
	}

void AudioEngineAnja::waitForFrame() noexcept
	{m_ready->wait();}

void AudioEngineAnja::waitForRecordComplete() noexcept
	{m_record_complete->wait();}


void AudioEngineAnja::onActivate(AudioConnection& source)
	{
	reset();

	if(m_multioutput)
		{
		char port_name_buffer[8];
		for(int k=0;k<16;++k)
			{
			sprintf(port_name_buffer,"Ch %u",k);
			source.audioPortOutputAdd(port_name_buffer);
			}
		}

	source.audioPortOutputAdd("Master out")
		.audioPortInputAdd("Audio capture")
		.midiPortInputAdd("MIDI in")
		.midiPortOutputAdd("MIDI out");

	m_sample_rate=source.sampleRateGet();
	m_fader_filter_factor=timeConstantToDecayFactor(1e-3,m_sample_rate);
	m_master_gain_out=m_master_gain_in;

	m_rec_buffers=new RecordBuffers(m_sample_rate); //Use 1 s for record buffers;
	m_record_task.recordBuffersSet(m_rec_buffers).disable().stopReset();

	m_time_start=secondsToFrames(clockGet(),m_sample_rate);
	m_rec_thread=Thread::create(m_record_task);
	}

void AudioEngineAnja::onDeactivate(AudioConnection& source)
	{
	m_record_task.disable();
	m_record_complete->set();
	m_record_task.stop();
	m_rec_buffers->readySet();
	m_rec_thread->destroy();
	delete m_rec_buffers;
	m_sample_rate=0.0;
	}

void AudioEngineAnja::reset()
	{
	m_event_next={0,{MIDIConstants::StatusCodes::INVALID,0,0,0},0.0f};
	QueueElement e;
	e.event=m_event_next;

	//	Drain event queue
	while(!m_event_queue.empty())
		{
		m_event_queue.pop_front();
		}

	//	Reset channel states
		{
		auto ptr_channel=m_channels.begin();
		auto ptr_channel_end=m_channels.end();
		while(ptr_channel!=ptr_channel_end)
			{
			*ptr_channel={1.0,1.0,1.0,1.0};
			++ptr_channel;
			}
		}

	//	Reset playback states
		{
		auto ptr=m_source_buffers.begin();
		auto ptr_end=m_source_buffers.end();
		while(ptr!=ptr_end)
			{
			if(ptr->valid())
				{ptr->release();}
			++ptr;
			}
		}

	//	Reset events
		{
		m_ready->reset();
		m_record_complete->reset();
		}
	}

void AudioEngineAnja::buffersAllocate(AudioConnection& source,unsigned int n_frames)
	{
	m_buffer_temp=ArraySimple<float>(n_frames);
	m_buffers_out=ArraySimple<float>(n_frames*m_channels.length());
	}


void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,uint8_t value_1) noexcept
	{
	if(m_sample_rate > 0.0)
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
	else
		{m_record_complete->set();}
	}

void AudioEngineAnja::eventPost(uint8_t status,uint8_t value_0,float value_1) noexcept
	{
	if(m_sample_rate > 0.0)
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
	else
		{m_record_complete->set();}
	}

void AudioEngineAnja::eventControlProcess(const AudioEngineAnja::Event& event
	,unsigned int time_offset) noexcept
	{
	switch(event.status_word[1])
		{
		case MIDIConstants::ControlCodes::CHANNEL_VOLUME:
			{
			auto value=event.status_word[3]==Event::VALUE_1_FLOAT?
				event.value : float(event.status_word[2])/127.0f;
			auto channel=event.status_word[0]&0xf;
			m_channels[channel].gain_in_current=value;
			}
			break;

		case MIDIConstants::ControlCodes::SOUND_1: //Fade out
			{
			auto value=event.status_word[3]==Event::VALUE_1_FLOAT?
				event.value : 4.096*Minifloat::fromBits(event.status_word[2])/15.5;
			auto channel=event.status_word[0]&0xf;
			m_channels[channel].fade_factor=timeConstantToDecayFactor(value,m_sample_rate);
			}
			break;

		case MIDIConstants::ControlCodes::SOUND_2: //Fade in
			{
			auto value=event.status_word[3]==Event::VALUE_1_FLOAT?
				event.value :  4.096*Minifloat::fromBits(event.status_word[2])/15.5;
			auto channel=event.status_word[0]&0xf;
			m_channels[channel].fade_factor=timeConstantToDecayFactor(-value,m_sample_rate);
			}
			break;

		case MIDIConstants::ControlCodes::SUSTAIN:
			{
			uint32_t channel=event.status_word[0]&0xf;
			auto ptr_voice=m_source_buffers.begin();
			auto voice_end=m_source_buffers.end();
			while(ptr_voice!=voice_end)
				{
				if(ptr_voice->valid() && ptr_voice->channelGet()==channel)
					{ptr_voice->sustainToggle();}
				++ptr_voice;
				}
			}
			break;

		case MIDIConstants::ControlCodes::GENERAL_PURPOSE_1: //Record start
			{
			auto slot=event.status_word[2];
			auto& waveform=(*r_waveforms)[slot];
		//	NOTE: Do not record to a slot that is in use, or has been
		//	marked as readonly.
			if(! ( waveform.flagsGet()
				&(Waveform::READONLY
				 |Waveform::PLAYBACK_RUNNING
				 |Waveform::RECORD_RUNNING) ) )
				{
				waveform.sampleRateSet(m_sample_rate)
					.flagsSet(Waveform::RECORD_RUNNING)
					.clear();
				m_rec_buffers->readySet();
				m_record_task.disable().waveformSet(waveform);
				m_rec_buffers->reset();
				m_record_task.enable();
				}
			}
			break;

		case MIDIConstants::ControlCodes::GENERAL_PURPOSE_2: //Record stop
			{
			auto slot=event.status_word[2];
			auto& waveform=(*r_waveforms)[slot];
		//	NOTE: Only try to stop the recording if the waveform is used
			if(waveform.flagsGet() & Waveform::RECORD_RUNNING)
				{
				m_rec_buffers->readySet();
				m_record_task.disable();
				}
			m_record_complete->set();
			}
			break;

		case MIDIConstants::ControlCodes::GENERAL_PURPOSE_3: //Record stop
			{
			m_rec_buffers->readySet();
			for(uint8_t k=0;k<128;++k)
				{
				auto& waveform=(*r_waveforms)[k];
			//	NOTE: Only stop the recording if the waveform is used
				if(waveform.flagsGet() & Waveform::RECORD_RUNNING)
					{
					m_record_task.disable();
					waveform.flagsUnset(Waveform::RECORD_RUNNING);
					auto ch=waveform.channelGet();

				//	Send a NOTE_ON event right now
					Event e
						{
						event.delay
							,{
							  uint8_t(MIDIConstants::StatusCodes::NOTE_ON|ch)
							 ,k
							 ,127
							 ,Event::VALUE_1_FLOAT
							}
						,1.0f
						};
					eventProcess(e,time_offset);
					}
				}
			}
			m_record_complete->set();
			break;
		}
	}

void AudioEngineAnja::eventProcess(const AudioEngineAnja::Event& event
	,unsigned int time_offset) noexcept
	{
	if(event.status_word[0]==MIDIConstants::StatusCodes::RESET)
		{
		auto buffer=m_source_buffers.begin();
		auto buffers_end=m_source_buffers.end();
		auto decay_factor=timeConstantToDecayFactor(1e-2,m_sample_rate);
		while(buffer!=buffers_end)
			{
			buffer->fadeOut(decay_factor);
			++buffer;
			}
		return;
		}

	switch(event.status_word[0]&0xf0)
		{
		case MIDIConstants::StatusCodes::NOTE_ON:
			{
			auto slot=event.status_word[1];
			auto& waveform=(*r_waveforms)[slot];
		//	NOTE: Do not try to play from a slot that is currently used for
		//	recording.
			if(! (waveform.flagsGet() & Waveform::RECORD_RUNNING ) )
				{
				auto voice=m_voice_current;
				auto velocity=event.status_word[3]==Event::VALUE_1_FLOAT?
					event.value : float(event.status_word[2])/127;

				m_source_buffers[voice].waveformSet(m_randgen,waveform,time_offset
					,velocity);
				r_source_buffers[slot]=voice;
				m_voice_current=(voice+1)%m_source_buffers.length();
				}
			}
			break;

		case MIDIConstants::StatusCodes::NOTE_OFF:
			{
		//	NOTE: The replace lock cannot be released here, due to the possible
		//	sustain flag. Instead, it has to be done in audioProcess, when
		//	the rendering loop detects that the clip should not continue.
			auto slot=event.status_word[1];
			auto& playback_buffer=m_source_buffers[ r_source_buffers[slot] ];

		//	NOTE: When note is stopped in sustain mode, ensure that the LOOP
		//	flag is is disabled to prevent infinite sound
			if(playback_buffer.flagsGet()&Waveform::SUSTAIN)
				{playback_buffer.flagsUnset(Waveform::LOOP);}
			else
				{playback_buffer.stop(time_offset);}
			}
			break;

		case MIDIConstants::StatusCodes::CONTROLLER:
			eventControlProcess(event,time_offset);
			break;
		}
	}

void AudioEngineAnja::eventWrite(AudioConnection& output_connection
	,AudioConnection::MIDIBufferOutputHandle midi_out
	,const AudioEngineAnja::Event& event
	,unsigned int time_offset) noexcept
	{
	if(event.status_word[0]!=MIDIConstants::StatusCodes::INVALID)
		{
		uint8_t value_0=event.status_word[0];
		uint8_t value_1=event.status_word[1];
		uint8_t value_2=event.status_word[2];

	//	Convert values suitable for MIDI
		switch(value_0&0xf0)
			{
			case MIDIConstants::StatusCodes::NOTE_OFF:
			case MIDIConstants::StatusCodes::NOTE_ON: // Rotate keys to get a more suitable octave
				value_1=(int(event.status_word[1]) + 36)%128;
				value_2=uint8_t(event.status_word[3]==Event::VALUE_1_FLOAT?
						event.value*127 : event.status_word[2]);
				break;

			case MIDIConstants::StatusCodes::CONTROLLER:
				value_1=event.status_word[1];
				switch(value_1)
					{
					case MIDIConstants::ControlCodes::CHANNEL_VOLUME:
						value_2=uint8_t(event.status_word[3]==Event::VALUE_1_FLOAT?
							(0.5f*event.value)*127 : event.status_word[2]);
						break;
				//	Filter out SOUND_1 and SOUND_2 controller code
					case MIDIConstants::ControlCodes::SOUND_1:
					case MIDIConstants::ControlCodes::SOUND_2:
						value_0=0;
						break;

					default:
						value_2=uint8_t(event.status_word[3]==Event::VALUE_1_FLOAT?
							event.value*127 : event.status_word[2]);
						break;
					}
				break;
			default:
				value_2=uint8_t(event.status_word[3]==Event::VALUE_1_FLOAT?
					event.value*127 : event.status_word[2]);
				break;
			}

		if(value_0!=0)
			{
			output_connection.midiEventWrite(midi_out,
				{
				 time_offset
				,value_0,value_1,value_2
				});
			}
		}
	}

void AudioEngineAnja::eventsUIFetch(AudioConnection& source,unsigned int n_frames) noexcept
	{
	auto output_buffer=source.midiBufferOutputGet(0,n_frames);
	Event event_next=m_event_next;
	auto& queue=m_event_queue;
	const auto now_in=secondsToFrames(clockGet(),m_sample_rate) - m_time_start;
	auto now=now_in;
	while(n_frames!=0)
		{
		if(now>=event_next.delay) // If next event has passed
			{
			eventWrite(source,output_buffer,event_next,now-now_in);
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
					eventWrite(source,output_buffer,event_next,now-now_in);
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
	m_event_next=event_next;
	}


AudioEngineAnja::Event AudioEngineAnja::eventConvert(const AudioConnection::MIDIEvent& e) noexcept
	{
	switch(e.data[0]&0xf0)
		{
		case MIDIConstants::StatusCodes::NOTE_OFF:
		case MIDIConstants::StatusCodes::NOTE_ON:
			{
			auto key_in=int(e.data[1]);
			auto key=key_in<36? (key_in-36)+128: key_in-36;
			return {0,{e.data[0],uint8_t(key),e.data[2],0}};
			}

		default:
			return {0,{e.data[0],e.data[1],e.data[2],0}};
		}
	}

void AudioEngineAnja::eventsRead(AudioConnection& source,unsigned int n_frames) noexcept
	{
 	auto events_in=source.midiBufferInputGet(0,n_frames);
	AudioConnection::MIDIEvent event_in;
	while(source.midiEventGet(events_in,event_in))
		{
		eventProcess( eventConvert(event_in), event_in.time_offset );
		event_in.data[0]=MIDIConstants::StatusCodes::INVALID;
		}
	}

void AudioEngineAnja::voicesRender(unsigned int n_frames) noexcept
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
			auto N=src_current->outputBufferGenerate(ptr_voice,n_frames);
			auto channel_index=src_current->channelGet();
			auto ptr_buffer_out=m_buffers_out.begin()+n_frames*channel_index;
			auto ptr_buffer_in=ptr_voice;
			while(N!=0)
				{
			//	TODO: We can vectorize here
				*ptr_buffer_out+=*ptr_buffer_in;
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

void AudioEngineAnja::channelsMix(AudioConnection& source,unsigned int n_frames) noexcept
	{
	auto N_ch=m_channels.length();
	auto multioutput=m_multioutput;
	auto buffer_out=source.audioBufferOutputGet(N_ch*multioutput,n_frames);
	memset(buffer_out,0,n_frames*sizeof(float));
	auto N_ch_in=N_ch;
	auto ptr_in=m_buffers_out.begin();
	auto channel=m_channels.begin();
	auto filterfactor=m_fader_filter_factor;
	float dummy;
	while(N_ch!=0)
		{
	//	TODO: This will only capture the most recent channel event within
	//	the current block. Also the event offset is not considered. It works
	//	as long as the buffer is small, but fails for larger buffers.

		auto N=n_frames;

		auto ptr_out=buffer_out;
	//	HACK Use the address of a dummy variable so we can write to
	//	*ptr_out_raw in any case
		float* ptr_out_raw=&dummy;
		if(multioutput)
			{ptr_out_raw=source.audioBufferOutputGet(N_ch_in-N_ch,N);}

		auto gain_in_current=channel->gain_in_current;
		auto gain_in_old=channel->gain_in_old;
		auto gain_out=channel->gain_out;
		auto fade_factor=channel->fade_factor;
		while(N!=0)
			{
			auto x=(*ptr_in) * gain_in_old * gain_out;
			*ptr_out_raw=x;
			*ptr_out+=x;
			++ptr_in;
			++ptr_out;
			ptr_out_raw=(ptr_out_raw==&dummy)?ptr_out_raw:ptr_out_raw + 1;
			--N;
			gain_out=std::max(1e-4,std::min(1.0,fade_factor*gain_out));
			gain_in_old=filterstep(gain_in_current,gain_in_old,filterfactor);
			}
		channel->gain_out=gain_out;
		channel->gain_in_old=gain_in_old;
		++channel;
		--N_ch;
		}
	}


void AudioEngineAnja::masterGainAdjust(AudioConnection& source
	,unsigned int n_frames) noexcept
	{
	auto filterfactor=m_fader_filter_factor;
	auto buffer_out=source.audioBufferOutputGet(m_channels.length()*m_multioutput
		,n_frames);
	auto gain_in=m_master_gain_in;
	auto gain_out=m_master_gain_out;
	while(n_frames!=0)
		{
		*buffer_out *= gain_out;
		gain_out=filterstep(gain_in,gain_out,filterfactor);
		++buffer_out;
		--n_frames;
		}
	m_master_gain_out=gain_out;
	}

void AudioEngineAnja::voicesStop() noexcept
	{
	auto src_begin=m_source_buffers.begin();
	auto src_current=src_begin;
	auto src_end=m_source_buffers.end();
	while(src_current!=src_end)
		{
		if(src_current->valid())
			{
			auto ch=src_current->channelGet();
			if(m_channels[ch].gain_out<1e-3
				&& m_channels[ch].fade_factor<1.0)
				{
				src_current->stop();
			//Reset fade factor so fade in will work after fade out
				m_channels[ch].fade_factor=1.0;
				}
			}
		++src_current;
		}
	}

void AudioEngineAnja::capture(AudioConnection& source,unsigned int n_frames) noexcept
	{
	if(m_record_task.enabled())
		{
		auto buffer_in=source.audioBufferInputGet(0,n_frames);
		m_rec_buffers->write(buffer_in,n_frames);
		}
	}

void AudioEngineAnja::audioProcess(AudioConnection& source,unsigned int n_frames) noexcept
	{
	eventsRead(source,n_frames);
	eventsUIFetch(source,n_frames);
	voicesRender(n_frames);
	channelsMix(source,n_frames);
	masterGainAdjust(source,n_frames);
	voicesStop();
	capture(source,n_frames);
	m_ready->set();
	}
