//@	{"targets":[{"name":"audioclient.o","type":"object","pkgconfig_libs":["jack"]}]}

#include "audioclient.hpp"
#include "../common/error.hpp"
#include <jack/jack.h>
#include <jack/midiport.h>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace Anja;

class AudioClient::Impl:private AudioClient
	{
	public:
		typedef jack_default_audio_sample_t SampleType;

		Impl(const char* name,void* cb_obj,const Vtable& vt);
		~Impl();


		void activate()
			{jack_activate(m_handle);}

		void midiInName(int index,const char* name)
			{portName<PortType::MIDI_IN>(index,name);}

		void midiOutName(int index,const char* name)
			{portName<PortType::MIDI_OUT>(index,name);}

		void waveInName(int index,const char* name)
			{portName<PortType::WAVE_IN>(index,name);}

		void waveOutName(int index,const char* name)
			{portName<PortType::WAVE_OUT>(index,name);}

		auto midiIn(int port,int n_frames) const noexcept
			{
			void* buffer=jack_port_get_buffer(m_ports[portOffset<PortType::MIDI_IN>(port)]
				,n_frames);
			auto n=jack_midi_get_event_count(buffer);
			std::pair<MidiEventIterator,MidiEventIterator> ret
				{
				 MidiEventIterator{buffer,0}
				,MidiEventIterator{buffer,static_cast<int>(n)}
				};
			return ret;
			}

		MidiMessageWriter midiOut(int port,int n_frames) const noexcept
			{
			void* buffer=jack_port_get_buffer(m_ports[portOffset<PortType::MIDI_OUT>(port)],n_frames);
			jack_midi_clear_buffer(buffer);
			return MidiMessageWriter(buffer);
			}

		const float* waveIn(int port,int n_frames) const noexcept
			{return reinterpret_cast<SampleType*>(jack_port_get_buffer(m_ports[portOffset<PortType::WAVE_IN>(port)],n_frames));}

		float* waveOut(int port,int n_frames) const noexcept
			{return reinterpret_cast<SampleType*>(jack_port_get_buffer(m_ports[portOffset<PortType::WAVE_OUT>(port)],n_frames));}

		int midiInCount() const noexcept
			{return portOffset<PortType::MIDI_OUT>(0);}

		int midiOutCount() const noexcept
			{return portOffset<PortType::WAVE_IN>(0) - portOffset<PortType::MIDI_OUT>(0);}

		int waveInCount() const noexcept
			{return portOffset<PortType::WAVE_OUT>(0) - portOffset<PortType::WAVE_IN>(0);}

		int waveOutCount() const noexcept
			{return m_ports.size() - portOffset<PortType::WAVE_OUT>(0);}

		double sampleRate() const noexcept
			{return m_sample_rate;}

		int portIndex(jack_port_t* port,PortType type) const noexcept
			{
			auto i=std::find_if(m_ports.begin(),m_ports.end(),[port](jack_port_t* p)
				{return port==p;});
			if(i==m_ports.end())
				{return -1;}
			auto ret=i-m_ports.begin();
			switch(type)
				{
				case PortType::MIDI_IN:
					return ret - portOffset<PortType::MIDI_IN>(0);
				case PortType::MIDI_OUT:
					return ret - portOffset<PortType::MIDI_OUT>(0);
				case PortType::WAVE_IN:
					return ret - portOffset<PortType::WAVE_IN>(0);
				case PortType::WAVE_OUT:
					return ret - portOffset<PortType::WAVE_OUT>(0);
				}
			return -1;
			}

		bool midiInConnected(int index) const noexcept
			{return connected(portOffset<PortType::MIDI_IN>(index));}

		bool midiOutConnected(int index) const noexcept
			{return connected(portOffset<PortType::MIDI_OUT>(index));}

		bool waveInConnected(int index) const noexcept
			{return connected(portOffset<PortType::WAVE_IN>(index));}

		bool waveOutConnected(int index) const noexcept
			{return connected(portOffset<PortType::WAVE_OUT>(index));}

		bool midiInEnum(void* cb_obj,PortEnumCallback cb);
		bool midiOutEnum(void* cb_obj,PortEnumCallback cb);
		bool waveInEnum(void* cb_obj,PortEnumCallback cb);
		bool waveOutEnum(void* cb_obj,PortEnumCallback cb);

		bool waveOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
		bool waveInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
		bool midiOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
		bool midiInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);


		void waveOutConnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::WAVE_OUT>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_connect(m_handle,source,port);
			}

		void waveOutDisconnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::WAVE_OUT>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_disconnect(m_handle,source,port);
			}

		void midiOutConnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::MIDI_OUT>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_connect(m_handle,source,port);
			}

		void midiOutDisconnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::MIDI_OUT>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_disconnect(m_handle,source,port);
			}

		void waveInConnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::WAVE_IN>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_connect(m_handle,port,source);
			}

		void waveInDisconnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::WAVE_IN>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_disconnect(m_handle,port,source);
			}

		void midiInConnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::MIDI_IN>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_connect(m_handle,port,source);
			}

		void midiInDisconnect(int index,const char* port)
			{
			auto offset=portOffset<PortType::MIDI_IN>(index);
			auto source=jack_port_name(m_ports[offset]);
			jack_disconnect(m_handle,port,source);
			}


	private:
		template<PortType type>
		int portOffset(int index) const noexcept
			{return m_port_type_offsets[ static_cast<int>(type) ] + index;}

		template<PortType type>
		void portName(int index,const char* name)
			{
			jack_port_rename(m_handle,m_ports[portOffset<type>(index)],name);
			}

		bool connected(int port) const noexcept
			{
			assert( static_cast<size_t>(port)<m_ports.size() );
			return jack_port_connected(m_ports[port]) > 0;
			}

		void* r_cb_obj;
		Vtable m_vt;
		jack_client_t* m_handle;
		double m_sample_rate;
		uint8_t m_port_type_offsets[PORT_TYPE_COUNT];
		std::vector<jack_port_t*> m_ports;

		bool portsEnum(void* cb_obj,PortEnumCallback cb,const char* port_type
			,JackPortFlags port_flags);

		bool connectionsEnum(int port_index,void* cb_obj,PortEnumCallback cb);
	};

AudioClient::AudioClient(const char* name,void* cb_obj,const Vtable& vt)
	{
	m_impl=new Impl(name,cb_obj,vt);
	}

AudioClient::~AudioClient()
	{delete m_impl;}


AudioClient& AudioClient::activate()
	{
	m_impl->activate();
	return *this;
	}

AudioClient& AudioClient::midiInName(int index,const char* name)
	{
	m_impl->midiInName(index,name);
	return *this;
	}

AudioClient& AudioClient::midiOutName(int index,const char* name)
	{
	m_impl->midiOutName(index,name);
	return *this;
	}

AudioClient& AudioClient::waveInName(int index,const char* name)
	{
	m_impl->waveInName(index,name);
	return *this;
	}

AudioClient& AudioClient::waveOutName(int index,const char* name)
	{
	m_impl->waveOutName(index,name);
	return *this;
	}

std::pair<AudioClient::MidiEventIterator,AudioClient::MidiEventIterator>
AudioClient::midiIn(int port,int n_frames) const noexcept
	{return m_impl->midiIn(port,n_frames);}

AudioClient::MidiEvent AudioClient::MidiEventIterator::operator[](int k) noexcept
	{
	jack_midi_event_t e;
	jack_midi_event_get(&e,r_buffer,k);

	auto msg=[&e]()
		{
		switch(e.size)
			{
			case 3:
				return MIDIConstants::Message(e.buffer[0],e.buffer[1],e.buffer[2]);
			case 2:
				return MIDIConstants::Message(e.buffer[0],e.buffer[1],0);
			case 1:
				return MIDIConstants::Message(e.buffer[0],0,0);
			default:
				return MIDIConstants::Message(0,0,0);
			}
		}();

	return MidiEvent(e.time,msg);
	}


AudioClient::MidiMessageWriter AudioClient::midiOut(int port,int n_frames) const noexcept
	{return m_impl->midiOut(port,n_frames);}


AudioClient::MidiMessageWriter& AudioClient::MidiMessageWriter::write(MIDIConstants::Message msg,int frame) noexcept
	{
	jack_midi_data_t data[3]={msg.statusRaw(),msg.value1(),msg.value2()};
	jack_midi_event_write(r_buffer,frame,data,3);
	return *this;
	}

const float* AudioClient::waveIn(int port,int n_frames) const noexcept
	{return m_impl->waveIn(port,n_frames);}

float* AudioClient::waveOut(int port,int n_frames) const noexcept
	{return m_impl->waveOut(port,n_frames);}

int AudioClient::midiInCount() const noexcept
	{return m_impl->midiInCount();}

int AudioClient::midiOutCount() const noexcept
	{return m_impl->midiOutCount();}


int AudioClient::waveInCount() const noexcept
	{return m_impl->waveInCount();}

int AudioClient::waveOutCount() const noexcept
	{return m_impl->waveOutCount();}

double AudioClient::sampleRate() const noexcept
	{return m_impl->sampleRate();}


bool AudioClient::midiInConnected(int index) const noexcept
	{return m_impl->midiInConnected(index);}

bool AudioClient::midiOutConnected(int index) const noexcept
	{return m_impl->midiOutConnected(index);}

bool AudioClient::waveInConnected(int index) const noexcept
	{return m_impl->waveInConnected(index);}

bool AudioClient::waveOutConnected(int index) const noexcept
	{return m_impl->waveOutConnected(index);}


bool AudioClient::midiInEnum(void* cb_obj,PortEnumCallback cb)
	{return m_impl->midiInEnum(cb_obj,cb);}

bool AudioClient::midiOutEnum(void* cb_obj,PortEnumCallback cb)
	{return m_impl->midiOutEnum(cb_obj,cb);}

bool AudioClient::waveInEnum(void* cb_obj,PortEnumCallback cb)
	{return m_impl->waveInEnum(cb_obj,cb);}

bool AudioClient::waveOutEnum(void* cb_obj,PortEnumCallback cb)
	{return m_impl->waveOutEnum(cb_obj,cb);}

bool AudioClient::waveOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{return m_impl->waveOutConnectionsEnum(index,cb_obj,cb);}

bool AudioClient::waveInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{return m_impl->waveInConnectionsEnum(index,cb_obj,cb);}

bool AudioClient::midiInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{return m_impl->midiInConnectionsEnum(index,cb_obj,cb);}

bool AudioClient::midiOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{return m_impl->midiOutConnectionsEnum(index,cb_obj,cb);}



void AudioClient::waveOutConnect(int index,const char* dest)
	{m_impl->waveOutConnect(index,dest);}

void AudioClient::waveOutDisconnect(int index,const char* dest)
	{m_impl->waveOutDisconnect(index,dest);}

void AudioClient::midiOutConnect(int index,const char* dest)
	{m_impl->midiOutConnect(index,dest);}

void AudioClient::midiOutDisconnect(int index,const char* dest)
	{m_impl->midiOutDisconnect(index,dest);}

void AudioClient::waveInConnect(int index,const char* dest)
	{m_impl->waveInConnect(index,dest);}

void AudioClient::waveInDisconnect(int index,const char* dest)
	{m_impl->waveInDisconnect(index,dest);}

void AudioClient::midiInConnect(int index,const char* dest)
	{m_impl->midiInConnect(index,dest);}

void AudioClient::midiInDisconnect(int index,const char* dest)
	{m_impl->midiInDisconnect(index,dest);}



static constexpr auto port_direction(AudioClient::PortType type)
	{return (static_cast<int>(type)&1) ? JackPortIsOutput : JackPortIsInput;}

static constexpr auto port_type(AudioClient::PortType type)
	{return (static_cast<int>(type)&2) ? JACK_DEFAULT_AUDIO_TYPE : JACK_DEFAULT_MIDI_TYPE;}

template<class Vtable>
static int ports_create(jack_client_t* client,const Vtable& vt,void* cb_obj
	,std::vector<jack_port_t*>& ports,AudioClient::PortType type)
	{
	int k=0;
	while(ports.size()!=256)
		{
		auto name=vt.port_callback(cb_obj,type,k);
		if(name==nullptr)
			{break;}
		ports.push_back( jack_port_register(client,name,port_type(type),port_direction(type),0) );
		++k;
		}
	return k;
	}


static AudioClient::PortType port_type(jack_port_t* port) noexcept
	{
	auto type_name=jack_port_type(port);
	auto output=jack_port_flags(port)&JackPortIsOutput;
	if(strcmp(type_name,JACK_DEFAULT_MIDI_TYPE)==0)
		{return output?AudioClient::PortType::MIDI_OUT : AudioClient::PortType::MIDI_IN;}
	else
		{return output?AudioClient::PortType::WAVE_OUT : AudioClient::PortType::WAVE_IN;}
	}


AudioClient::Impl::Impl(const char* name,void* cb_obj,const Vtable& vt):AudioClient(*this)
	,r_cb_obj(cb_obj),m_vt(vt)
	{
	jack_status_t status;
 	m_handle=jack_client_open(name,JackNoStartServer,&status);
	if(m_handle==NULL)
		{
		m_impl=nullptr;
		throw Error("The client \"",name,"\" failed to connect to JACK. Check if the server is running.");
		}

	jack_set_process_callback(m_handle,[](jack_nframes_t n_frames,void* obj)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		self->m_vt.process_callback(self->r_cb_obj,*self,n_frames);
		return 0;
		},this);

	jack_set_buffer_size_callback(m_handle,[](jack_nframes_t n_frames,void* obj)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		self->m_vt.buffersize_callback(self->r_cb_obj,*self,n_frames);
		return 0;
		},this);

	jack_set_port_connect_callback(m_handle,[](jack_port_id_t a,jack_port_id_t b,int connect,void* obj)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		auto port_a=jack_port_by_id(self->m_handle,a);
		if(jack_port_is_mine(self->m_handle,port_a))
			{
			auto type=port_type(port_a);
			auto i=self->portIndex(port_a,type);
			if(connect)
				{self->m_vt.port_connected(self->r_cb_obj,*self,port_type(port_a),i);}
			else
				{self->m_vt.port_disconnected(self->r_cb_obj,*self,port_type(port_a),i);}
			}

		auto port_b=jack_port_by_id(self->m_handle,b);
		if(jack_port_is_mine(self->m_handle,port_b))
			{
			auto type=port_type(port_b);
			auto i=self->portIndex(port_b,type);
			if(connect)
				{self->m_vt.port_connected(self->r_cb_obj,*self,port_type(port_b),i);}
			else
				{self->m_vt.port_disconnected(self->r_cb_obj,*self,port_type(port_b),i);}
			}
		},this);

	m_port_type_offsets[0]=0;
	m_port_type_offsets[1]=m_port_type_offsets[0] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_IN);
	m_port_type_offsets[2]=m_port_type_offsets[1] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_OUT);
	m_port_type_offsets[3]=m_port_type_offsets[2] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_IN);
	ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_OUT);
	m_sample_rate=jack_get_sample_rate(m_handle);
	}

AudioClient::Impl::~Impl()
	{
	jack_deactivate(m_handle);
	jack_set_process_callback(m_handle,nullptr,nullptr);
	m_impl=nullptr;
	std::for_each(m_ports.begin(),m_ports.end(),[this](jack_port_t* port)
		{jack_port_unregister(m_handle,port);});
	jack_client_close(m_handle);
	}

bool AudioClient::Impl::midiInEnum(void* cb_obj,PortEnumCallback cb)
	{return portsEnum(cb_obj,cb,port_type(PortType::MIDI_IN),port_direction((PortType::MIDI_IN)));}

bool AudioClient::Impl::midiOutEnum(void* cb_obj,PortEnumCallback cb)
	{return portsEnum(cb_obj,cb,port_type(PortType::MIDI_OUT),port_direction((PortType::MIDI_OUT)));}

bool AudioClient::Impl::waveInEnum(void* cb_obj,PortEnumCallback cb)
	{return portsEnum(cb_obj,cb,port_type(PortType::WAVE_IN),port_direction((PortType::WAVE_IN)));}

bool AudioClient::Impl::waveOutEnum(void* cb_obj,PortEnumCallback cb)
	{return portsEnum(cb_obj,cb,port_type(PortType::WAVE_OUT),port_direction((PortType::WAVE_OUT)));}

bool AudioClient::Impl::portsEnum(void* cb_obj,PortEnumCallback cb,const char* port_type
	,JackPortFlags port_flags)
	{
	auto ports=jack_get_ports(m_handle,nullptr,port_type,port_flags);
	if(ports==nullptr)
		{return 0;}
	try
		{
		auto port=ports;
		while(*port!=nullptr)
			{
			if(!cb(cb_obj,*this,*port))
				{
				free(ports);
				return 0;
				}
			++port;
			}
		free(ports);
		return 1;
		}
	catch(...)
		{
		free(ports);
		throw;
		}
	}

bool AudioClient::Impl::waveOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{
	auto port_index=portOffset<PortType::WAVE_OUT>(index);
	return connectionsEnum(port_index,cb_obj,cb);
	}

bool AudioClient::Impl::waveInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{
	auto port_index=portOffset<PortType::WAVE_IN>(index);
	return connectionsEnum(port_index,cb_obj,cb);
	}

bool AudioClient::Impl::midiOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{
	auto port_index=portOffset<PortType::MIDI_OUT>(index);
	return connectionsEnum(port_index,cb_obj,cb);
	}

bool AudioClient::Impl::midiInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb)
	{
	auto port_index=portOffset<PortType::MIDI_IN>(index);
	return connectionsEnum(port_index,cb_obj,cb);
	}


bool AudioClient::Impl::connectionsEnum(int port_index,void* cb_obj,PortEnumCallback cb)
	{
	auto connections=jack_port_get_all_connections(m_handle,m_ports[port_index]);
	if(connections==nullptr)
		{return 0;}
	try
		{
		auto connection=connections;
		while(*connection!=nullptr)
			{
			if(!cb(cb_obj,*this,*connection))
				{
				free(connections);
				return 0;
				}
			++connection;
			}
		free(connections);
		return 1;
		}
	catch(...)
		{
		free(connections);
		throw;
		}
	}
