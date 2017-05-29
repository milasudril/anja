//@	{"targets":[{"name":"engine.o","type":"object"}]}

#include "engine.hpp"

using namespace Anja;

static String client_name(const String& str)
	{
	auto ret=str;
	ret.append(".anja");
	return std::move(ret);
	}

Engine::Engine(const Session& session):r_session(&session)
	,m_running(1)
	,m_client(client_name(session.titleGet()).begin(),*this)
	,m_rec_thread(*this,TaskId::RECORD)
	{}

Engine::~Engine()
	{
	m_running=0;
	m_ready.set();
	}

void Engine::process(AudioClient& client,int32_t n_frames) noexcept
	{
	auto midi_in=client.midiIn(0,n_frames);
	std::for_each(midi_in.first,midi_in.second,[](const AudioClient::MidiEvent& e)
		{
		printf("%d    %x %x %x\n",e.time_offset,e.message.status()
			,e.message.value1(),e.message.value2());
		});
	}

static void record(Engine& engine)
	{
	while(engine.running())
		{
		engine.readyWait();
		}
	}

void Engine::run(TaskId id)
	{
	switch(id)
		{
		case TaskId::RECORD:
			record(*this);
			break;
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
