//@	{"targets":[{"name":"engine.o","type":"object"}]}

#include "engine.hpp"
#include "message.hpp"

using namespace Anja;

static String client_name(const String& str)
	{
	auto ret=str;
	ret.append(".anja");
	return std::move(ret);
	}

Engine::Engine(const Session& session):r_session(&session)
	,m_client(client_name(session.titleGet()).begin(),*this)
	{}

void Engine::process(AudioClient& client,int32_t n_frames) noexcept
	{}

bool Engine::port(int index,AudioClient::PortInfo& info) const
	{
	switch(index)
		{
		case 0:
			info={"MIDI in",AudioClient::PortType::MIDI_IN};
			return 1;
		case 1:
			info={"MIDI out",AudioClient::PortType::MIDI_OUT};
			return 1;
		case 2:
			info={"Wave in",AudioClient::PortType::WAVE_IN};
			return 1;
		case 3:
			if(!(r_session->flagsGet()&Session::MULTIOUTPUT))
				{
				info={"Master out",AudioClient::PortType::WAVE_OUT};
				return 1;
				}
		}
	if(!(r_session->flagsGet()&Session::MULTIOUTPUT))
		{return 0;}

	index-=3;
	if(index>=0 && index<16)
		{
		info={r_session->channelLabelGet(index).begin(),AudioClient::PortType::WAVE_OUT};
		return 1;
		}
	if(index==16)
		{
		info={"Master out",AudioClient::PortType::WAVE_OUT};
		return 1;
		}

	return 0;
	}
