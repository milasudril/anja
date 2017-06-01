//@	{"targets":[{"name":"engine.o","type":"object"}]}

#include "engine.hpp"

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
	,m_client(client_name(session.titleGet()).begin(),*this)
	,m_rec_thread(*this,TaskType<Engine::TaskId::RECORD>{})
	{}

Engine::~Engine()
	{
	m_running=0;
	m_ready.set();
	}

static bool expired(AudioClient::MidiEvent e,double time_factor
	,double now)
	{
	return e.message.valid() && time_factor*e.time_offset<=now;
	}

void Engine::process(AudioClient& client,int32_t n_frames) noexcept
	{
	auto time_factor=48000.0/1000.0; //TODO: Do not hard-code sample rate
	auto now=time_factor*(now_ms() - m_time_init);
	auto midi_in=client.midiIn(0,n_frames);
	auto midi_out=client.midiOut(0,n_frames);
	auto event_current=m_event_last;
	if(client.waveOutCount()==2) //Two outputs => single-channel output (Master + Audition)
		{
		auto master=client.waveOut(0,n_frames);
		auto audition=client.waveOut(1,n_frames);

		for(int32_t k=0;k<n_frames;++k)
			{
			if(expired(event_current,time_factor,now + k))
				{
			/*	printf("Fire %d %d %d\n",event_current.message.statusRaw()
					,event_current.message.value1()
					,event_current.message.value2());*/
				midi_out.write(event_current.message,k);
				event_current.message.clear();
				}
			while(!m_ui_events.empty())
				{
				event_current=m_ui_events.pop_front();
				if(expired(event_current,time_factor,now + k))
					{
					midi_out.write(event_current.message,k);
				/*	printf("Fire %d %d %d\n",event_current.message.statusRaw()
						,event_current.message.value1()
						,event_current.message.value2());*/
					event_current.message.clear();
					}
				else
					{break;}
				}
			}
		}

	m_event_last=event_current;
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
