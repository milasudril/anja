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

Engine::Engine(Session& session):r_session(&session)
	,m_running(1)
	,m_ui_events(1024)
	,m_time_init(now_ms())
	,m_voices(64)
	,m_key_to_voice_index(128)
	,m_voices_alloc(m_voices.length())
	,m_channel_buffers(16*64)
	,m_channel_gain(16)
	,m_rec_buffers(48000)
	,m_rec_write_offset(0)
	,m_ch_state(0xffff)
	,r_waveform_rec(nullptr)
	,m_client(client_name(session.titleGet()).begin(),*this)
	,m_rec_thread(*this,TaskType<Engine::TaskId::RECORD>{})
	{
	m_voices_alloc.fill();
	std::fill(m_key_to_voice_index.begin(),m_key_to_voice_index.end()
		,m_voices_alloc.null());
	std::fill(m_channel_gain.begin<1>(),m_channel_gain.end<1>()
		,std::pair<Vec4d,Vec4d>{Vec4d{1.0,1.0,1.0,1.0},Vec4d{1.0,1.0,1.0,1.0}});

	portConnected(m_client,AudioClient::PortType::MIDI_OUT,0);
	if(m_client.sampleRate()!=48000) //Use one second record buffers;
		{m_rec_buffers=ArrayMultiSimple<float,float>(m_client.sampleRate());}
	}

void Engine::init_notify()
	{
	for(int k=0;k<16;++k)
		{m_vt.unmuted(r_cb_obj,*this,k);}
	}

void Engine::bufferSize(AudioClient& client,int n_frames)
	{
	m_channel_buffers=ArraySimple<float>(n_frames*m_channel_gain.length());
	}

Engine::~Engine()
	{
	recordStop();
	for(int k=0;k<16;++k)
		{messagePost(MIDI::Message{MIDI::ControlCodes::SOUND_OFF,k,0});}

//	Poll event queue state to make sure there are no more events left when we
//	destroy the engine.
	while(!m_ui_events.empty());
	m_running=0;
	m_ready.set();
	}

void Engine::portConnected(AudioClient& client,AudioClient::PortType type,int index)
	{
	if(type==AudioClient::PortType::MIDI_OUT)
		{
		for(int k=0;k<16;++k)
			{
			messagePost(MIDI::Message(MIDI::ControlCodes::SOUND_OFF,k,0));
			channelGain(k,r_session->channel(k).gain());
			}
		}
	}

void Engine::playbackDone(Voice& voice,int event_offset) noexcept
	{
	m_voices_alloc.idRelease(voice.id());
	r_session->waveformGet(midiToSlot(voice.key())).unlock();
	voice=Voice{}; //Reset voice so we do not get here any more.
	}

void Engine::loop(Voice& voice,int event_offset) noexcept
	{
	if(voice.flags()&Waveform::LOOP)
		{
		if(voice.flags()&Waveform::GAIN_ONLOOP_SET)
			{voice.gainRandomize(m_rng);}
		voice.playFromLoopBegin();
		}
	}

void Engine::process(MIDI::Message msg,int offset,double fs) noexcept
	{
	switch(msg.status())
		{
		case MIDI::StatusCodes::NOTE_OFF:
			{
			//TODO: Same note different channels should be OK?
			auto i=m_key_to_voice_index[ msg.value1() ];
			if(i!=m_voices_alloc.null())
				{
				m_voices[i].stop(offset);
				m_key_to_voice_index[ msg.value1() ]=m_voices_alloc.null();
				}
			}
			break;

		case MIDI::StatusCodes::NOTE_ON:
			//TODO: Same note different channels should be OK?
			if(m_key_to_voice_index[ msg.value1()&0x7f ]==m_voices_alloc.null())
				{
				auto& waveform=r_session->waveformGet(midiToSlot(msg.value1()&0x7f));
				if(waveform.lockTry())
					{
					auto i=m_voices_alloc.idGet();
					if(i==m_voices_alloc.null())
						{
						m_voices_alloc.reset();
						i=m_voices_alloc.idGet();
						}
					assert(i!=m_voices_alloc.null());
					m_key_to_voice_index[ msg.value1()&0x7f ]=i;

					m_voices[i]=Voice(waveform
						,msg.value1()&0x80?
							 17
							:(r_session->flagsGet()&Session::ALLOW_CHANNEL_OVERRIDE)?
								 msg.channel()
								:waveform.channel()
						,msg.value2()/127.0,offset,*this,i,msg.value1()&0x7f);
					m_voices[i].gainRandomize(m_rng);
					}
				}
			break;

		case RECORD_STOP:
			printf("Record stop\n");
			r_waveform_rec=nullptr;
			break;

		case MIDI::StatusCodes::CONTROL_CHANGE:
			switch(msg.ctrlCode())
				{
				case MIDI::ControlCodes::CHANNEL_VOLUME:
					m_channel_gain.get<0>(msg.channel())=dBToAmplitude(MIDI_val_to_dB(msg.value2()));
					break;

				case MIDI::ControlCodes::SOUND_OFF:
					std::for_each(m_voices.begin(),m_voices.end(),[msg,offset,this](Voice& voice)
						{
						if(!voice.done() && voice.channel()==msg.channel())
							{
							voice.flagsUnset(Waveform::SUSTAIN).stop(offset);
							m_key_to_voice_index[voice.key()]=m_voices_alloc.null();
							}
						});
					break;

				case FADE_IN:
					{
					auto t=MIDI_val_to_sec(msg.value2());
					auto f=1.0/sec_to_decay_factor(t,fs);
					auto f2=f*f;
					auto f4=f2*f2;
					m_channel_gain.get<1>(msg.channel()).first=Vec4d{1.0,f,f2,f2*f};
					m_channel_gain.get<1>(msg.channel()).first*=Vec4d{1.0e-4,1.0e-4,1.0e-4,1.0e-4};
					m_channel_gain.get<1>(msg.channel()).second=Vec4d{f4,f4,f4,f4};
					}
					break;

				case FADE_OUT:
					{
					auto t=MIDI_val_to_sec(msg.value2());
					auto f=sec_to_decay_factor(t,fs);
					auto f2=f*f;
					auto f4=f2*f2;
					m_channel_gain.get<1>(msg.channel()).first=Vec4d{1.0,f,f2,f2*f};
					m_channel_gain.get<1>(msg.channel()).second=Vec4d{f4,f4,f4,f4};
					}
					break;

				case RECORD_START:
					{
					printf("Record start %d\n",msg.value2());
					r_waveform_rec=&r_session->waveformGet(midiToSlot(msg.value2()&0x7f));
					}
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
	auto fs=client.sampleRate();
	auto time_factor=fs/1000.0;
	auto now=time_factor*(now_ms() - m_time_init);

	auto midi_out=client.midiOut(0,n_frames);

//	Fetch events
		{
	//	From UI input queue
		auto event_current=m_event_last;
			if(event_current.valid())
				{
				auto offset=std::max(time_factor*event_current.timeOffset() - now,0.0);
				process(event_current.message(),static_cast<int>(offset),fs);
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
					process(event_current.message(),static_cast<int>(offset),fs);
					event_current.clear();
					}
				else
					{break;}
				}
		m_event_last=event_current;
		}
		{
	//	From MIDI in port
		auto midi_in=client.midiIn(0,n_frames);
		std::for_each(midi_in.first,midi_in.second,[this,fs](AudioClient::MidiEvent e)
			{process(e.message(),e.timeOffset(),fs);});
		}

//	Render and mix voices
		{
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
		}

//	Apply channel gain
		{
		assert(n_frames%4==0);
		for(size_t k=0;k<m_channel_gain.length();++k)
			{
			auto gain=m_channel_gain.get<0>(k);
			auto f=m_channel_gain.get<1>(k);
			vec4_t<float> g_vec={gain,gain,gain,gain};
			auto begin=reinterpret_cast< vec4_t<float>* >(m_channel_buffers.begin() + k*n_frames);
			auto end=begin + n_frames/4;
			constexpr auto clamp_a=Vec4d{0.0,0.0,0.0,0.0};
			constexpr auto clamp_b=Vec4d{1.0,1.0,1.0,1.0};
			while(begin!=end)
				{
				*begin=static_cast<vec4_t<float>>(f.first)*g_vec*(*begin);
				f.first*=f.second;
				f.first=clamp(clamp_a,clamp_b,f.first);
				++begin;
				}
			if(f.first[3] < 1.0e-4 && m_ch_state&(1<<k))
				{
				m_ch_state&=~(1<<k);
				m_vt.muted(r_cb_obj,*this,k);
				auto msg=MIDI::Message{MIDI::ControlCodes::SOUND_OFF,static_cast<int>(k),0};
				process(msg,n_frames-1,fs);
				write(msg,n_frames-1,midi_out);
				}
			else
			if(f.first[3] >= 1.0e-4 && !(m_ch_state&(1<<k)))
				{
				m_ch_state|=(1<<k);
				m_vt.unmuted(r_cb_obj,*this,k);
				}
			m_channel_gain.get<1>(k).first=f.first;
			}
		}


	auto master=reinterpret_cast<vec4_t<float>*>( client.waveOut(indexMaster(client),n_frames) );

//	Mix channels
		{
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
		}

//	Adjust master gain
		{
		auto g=dBToAmplitude( r_session->gainGet() );
		auto g_vec=vec4_t<float>{g,g,g,g};
		std::for_each(master,master + n_frames/4,[g_vec](vec4_t<float>& vec)
			{vec*=g_vec;});
		}

//	For individual channel output, write data to output ports
	if(client.waveOutCount()>2)
		{
		auto ch_buffers=m_channel_buffers.begin();
		for(size_t k=0;k<m_channel_gain.length();++k)
			{memcpy(client.waveOut(k,n_frames),ch_buffers + k*n_frames,n_frames*sizeof(float));}
		}

//	Write input data to record buffers
		{
		auto wave_in=m_client.waveIn(0,n_frames);
		auto n=std::min(static_cast<size_t>(n_frames),m_rec_buffers.length()-m_rec_write_offset);
		memcpy(m_rec_buffers.begin<0>(),wave_in,n*sizeof(float));
		m_rec_write_offset+=n;
		if(static_cast<size_t>(m_rec_write_offset)==m_rec_buffers.length())
			{
			m_rec_buffers.swap<0,1>();
			m_rec_write_offset=0;
			m_ready.set();
			}
		}
	}

template<>
void Engine::run<Engine::TaskId::RECORD>()
	{
	Waveform* r_waveform=nullptr;
	while(m_running)
		{
		m_ready.wait();
		auto r_waveform_in=r_waveform_rec;
		if(r_waveform==nullptr && r_waveform_in!=nullptr)
			{
			if(r_waveform_in->lockTry())
				{
				r_waveform=r_waveform_in;
				r_waveform->clear();
				printf("Locked resource %p for REC\n",r_waveform);
				}
			}

		if(r_waveform!=nullptr)
			{
			r_waveform->append(m_rec_buffers.begin<1>(),m_rec_buffers.length());
			if(r_waveform_in==nullptr)
				{
				printf("Unlocking resource %p for REC\n",r_waveform);
				r_waveform->offsetsReset().unlock();
				r_waveform=nullptr;
				}
			}
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
