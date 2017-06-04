//@	{"targets":[{"name":"engine.o","type":"object"}]}

#include "engine.hpp"
#include "../sessiondata/keymap.hpp"
#include "../common/units.hpp"

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
	,m_channel_buffers(16*64)
	,m_channel_gain(16)
	,m_client(client_name(session.titleGet()).begin(),*this)
	,m_rec_thread(*this,TaskType<Engine::TaskId::RECORD>{})
	{
	m_voices_alloc.fill();
	std::fill(m_key_to_voice_index.begin(),m_key_to_voice_index.end()
		,m_voices_alloc.null());

	portConnected(m_client,AudioClient::PortType::MIDI_OUT,0);
	}

void Engine::bufferSize(AudioClient& client,int n_frames) noexcept
	{
	m_channel_buffers=ArraySimple<float>(n_frames*m_channel_gain.length());
	}

Engine::~Engine()
	{
	m_running=0;
	m_ready.set();
	}

void Engine::portConnected(AudioClient& client,AudioClient::PortType type,int index) noexcept
	{
	if(type==AudioClient::PortType::MIDI_OUT)
		{
		for(int k=0;k<16;++k)
			{
			messagePost(MIDI::Message(MIDI::ControlCodes::SOUND_OFF,k,0));
			channelGain(k,r_session->channelGet(k).gainGet());
			}
		}
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

		case MIDI::StatusCodes::CONTROL_CHANGE:
			switch(msg.ctrlCode())
				{
				case MIDI::ControlCodes::CHANNEL_VOLUME:
					m_channel_gain[msg.channel()]=dBToAmplitude(MIDI_val_to_dB(msg.value2()));
					break;
				default:
					break;
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

int Engine::indexAudition(const AudioClient& client) const noexcept
	{return indexMaster(client) + 1;}

int Engine::indexMaster(const AudioClient& client) const noexcept
	{
	return client.waveOutCount()>2?16:0;
	}

void Engine::process(AudioClient& client,int n_frames) noexcept
	{
	auto time_factor=client.sampleRate()/1000.0;
	auto now=time_factor*(now_ms() - m_time_init);

	auto midi_out=client.midiOut(0,n_frames);

//	Fetch events
	auto midi_in=client.midiIn(0,n_frames);
	auto event_current=m_event_last;
		if(event_current.valid())
			{
			auto offset=std::max(time_factor*event_current.timeOffset() - now,0.0);
			process(event_current.message(),static_cast<int>(offset));
			write(event_current.message(),static_cast<int>(offset),midi_out);
			event_current.clear();
			}
		while(!m_ui_events.empty())
			{
			event_current=m_ui_events.pop_front();
			if(event_current.valid() && n_frames + time_factor*event_current.timeOffset() - now<n_frames)
				{
				auto offset=std::max(n_frames + time_factor*event_current.timeOffset() - now,0.0);
				write(event_current.message(),static_cast<int>(offset),midi_out);
				process(event_current.message(),static_cast<int>(offset));
				event_current.clear();
				}
			else
				{break;}
			}
	m_event_last=event_current;

//	Render and mix voices
	memset(m_channel_buffers.begin(),0,m_channel_buffers.length()*sizeof(float));
	auto audition=client.waveOut(indexAudition(client),n_frames);
	memset(audition,0,n_frames*sizeof(*audition));
	std::for_each(m_voices.begin(),m_voices.end(),[audition,n_frames,this](Voice& v)
		{
		if( !v.done() )
			{
			if(v.channel()==17)
				{v.generate(audition,n_frames);}
			else
				{v.generate(m_channel_buffers.begin() + n_frames*v.channel(),n_frames);}
			}
		});

//	Apply channel gain
	assert(n_frames%4==0);
	for(size_t k=0;k<m_channel_gain.length();++k)
		{
		auto gain=m_channel_gain[k];
		vec4_t<float> g_vec={gain,gain,gain,gain};
		auto begin=reinterpret_cast< vec4_t<float>* >(m_channel_buffers.begin() + k*n_frames);
		auto end=begin + n_frames/4;
		while(begin!=end)
			{
			*begin=g_vec*(*begin);
			++begin;
			}
		}

//	Mix channels
	auto master=reinterpret_cast<vec4_t<float>*>( client.waveOut(indexMaster(client),n_frames) );
	memset(master,0,n_frames*sizeof(float));
	for(size_t k=0;k<m_channel_gain.length();++k)
		{
		auto begin=reinterpret_cast< vec4_t<const float>* >(m_channel_buffers.begin() + k*n_frames);
		auto ptr_out=master;
		auto end=begin + n_frames/4;
		while(begin!=end)
			{
			*ptr_out+=*begin;
			++begin;
			++ptr_out;
			}
		}

	auto g=dBToAmplitude( r_session->gainGet() );
	auto g_vec=vec4_t<float>{g,g,g,g};
	std::for_each(master,master + n_frames/4,[g_vec](vec4_t<float>& vec)
		{vec*=g_vec;});


	if(client.waveOutCount()>2)  //Individual channel output
		{
		auto ch_buffers=m_channel_buffers.begin();
		for(size_t k=0;k<m_channel_gain.length();++k)
			{memcpy(client.waveOut(k,n_frames),ch_buffers + k*n_frames,n_frames*sizeof(float));}
		}
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
