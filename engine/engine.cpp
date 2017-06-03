//@	{"targets":[{"name":"engine.o","type":"object"}]}

#include "engine.hpp"
#include "../sessiondata/keymap.hpp"

using namespace Anja;

static String client_name(const String& str)
	{
	auto ret=str;
	ret.append(".anja");
	return std::move(ret);
	}

namespace
	{
	template<Engine::TaskId x>
	struct TaskType
		{static constexpr Engine::TaskId value=x;};
	}

Engine::Engine(const Session& session):r_session(&session)
	,m_running(1)
	,m_ui_events(1024)
	,m_time_init(now_ms())
	,m_voices(64)
	,m_key_to_voice_index(128)
	,m_voices_alloc(m_voices.length())
	,m_client(client_name(session.titleGet()).begin(),*this)
	,m_rec_thread(*this,TaskType<Engine::TaskId::RECORD>{})
	{
	m_voices_alloc.fill();
	std::fill(m_key_to_voice_index.begin(),m_key_to_voice_index.end()
		,m_voices_alloc.null());
	}

Engine::~Engine()
	{
	m_running=0;
	m_ready.set();
	}

void Engine::process(MIDI::Message msg,int offset) noexcept
	{
	switch(msg.status())
		{
		case MIDI::StatusCodes::NOTE_OFF:
			{
			auto i=m_key_to_voice_index[ msg.value1() ];
			if(i!=m_voices_alloc.null())
				{
				m_voices[i].stop(offset);
				m_key_to_voice_index[ msg.value1() ]=m_voices_alloc.null();
				//TODO: This should not be done until the waveform has been played
				m_voices_alloc.idRelease(i);
				}
			}
			break;

		case MIDI::StatusCodes::NOTE_ON:
			{
			auto i=m_voices_alloc.idGet();
			if(i==m_voices_alloc.null())
				{
				m_voices_alloc.reset();
				i=m_voices_alloc.idGet();
				}

			assert(i!=m_voices_alloc.null());
			m_key_to_voice_index[ msg.value1()&0x7f ]=i;
			m_voices[i]=Voice(r_session->waveformGet(midiToSlot(msg.value1()&0x7f))
				,msg.value1()&0x80?17:msg.channel()
				,msg.value2()/127.0,offset);
			}
			break;

		default:
			break;
		}
	}

static void write(MIDI::Message msg,int offset,AudioClient::MidiMessageWriter& writer)
	{
	if(!(msg.value1()&0x80))
		{writer.write(msg,offset);}
	}

void Engine::process(AudioClient& client,int n_frames) noexcept
	{
	auto time_factor=client.sampleRate()/1000.0;
	auto now=time_factor*(now_ms() - m_time_init);

	auto midi_in=client.midiIn(0,n_frames);
	auto midi_out=client.midiOut(0,n_frames);
	auto event_current=m_event_last;


	for(int32_t k=0;k<n_frames;++k)
		{
		auto t_samp=now + k;
		if(event_current.valid() && time_factor*event_current.timeOffset()<=t_samp)
			{
			auto offset=std::max(time_factor*event_current.timeOffset() - t_samp,0.0);
		//	printf("(a) Offset %.15g (k=%d)\n", offset,k);
			process(event_current.message(),static_cast<int>(offset));
			write(event_current.message(),static_cast<int>(offset),midi_out);
			event_current.clear();
			}
		while(!m_ui_events.empty())
			{
			event_current=m_ui_events.pop_front();
			if(event_current.valid() && time_factor*event_current.timeOffset()<t_samp)
				{
				auto offset=n_frames + time_factor*event_current.timeOffset() - t_samp;
			//	printf("(b) Offset %.15g (k=%d)\n",offset,k);
				write(event_current.message(),static_cast<int>(offset),midi_out);
				process(event_current.message(),static_cast<int>(offset));
				event_current.clear();
				}
			else
				{break;}
			}
		}

	m_event_last=event_current;

	if(client.waveOutCount()==2) //Two outputs => single-channel output (Master + Audition)
		{
		auto master=client.waveOut(0,n_frames);
		auto audition=client.waveOut(1,n_frames);
		memset(audition,0,n_frames*sizeof(*audition));
		std::for_each(m_voices.begin(),m_voices.end(),[audition,n_frames,this](Voice& v)
			{
			if(!v.done() && v.channel()==17)
				{
				v.generate(audition,n_frames);
				}
			});
		}
	if(n_frames>=1024)
		{puts("=");}
	}

template<>
void Engine::run<Engine::TaskId::RECORD>()
	{
	while(m_running)
		{
		m_ready.wait();
		}
	}

const char* Engine::port(AudioClient::PortType type,int index) const noexcept
	{
	switch(type)
		{
		case AudioClient::PortType::MIDI_IN:
			return index==0?"MIDI in":nullptr;
		case AudioClient::PortType::MIDI_OUT:
			return index==0?"MIDI out":nullptr;
		case AudioClient::PortType::WAVE_IN:
			return index==0?"Wave in":nullptr;
		case AudioClient::PortType::WAVE_OUT:
			if(r_session->flagsGet()&Session::MULTIOUTPUT)
				{
				if(index>=0 && index<16)
					{return r_session->channelLabelGet(index).begin();}
				if(index==16)
					{return "Master out";}
				if(index==17)
					{return "Audition";}
				return nullptr;
				}
			else
				{
				switch(index)
					{
					case 0:
						return "Master out";
					case 1:
						return "Audition";
					default:
						return nullptr;
					}
				}
			break;

		}
	return nullptr;
	}
