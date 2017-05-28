//@	{"targets":[{"name":"audioclient.o","type":"object","pkgconfig_libs":["jack"]}]}

#include "audioclient.hpp"
#include "../common/error.hpp"
#include <jack/jack.h>
#include <jack/midiport.h>
#include <vector>
#include <algorithm>

using namespace Anja;

class AudioClient::Impl:private AudioClient
	{
	public:
		Impl(const char* name,void* cb_obj,const Vtable& vt);
		~Impl();


		void midiInName(int index,const char* name)
			{portName<PortType::MIDI_IN>(index,name);}

		void midiOutName(int index,const char* name)
			{portName<PortType::MIDI_OUT>(index,name);}

		void waveInName(int index,const char* name)
			{portName<PortType::WAVE_IN>(index,name);}

		void waveOutName(int index,const char* name)
			{portName<PortType::WAVE_OUT>(index,name);}

		auto midiEvents(int port,int n_frames) noexcept
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

	private:
		template<PortType type>
		int portOffset(int index) noexcept
			{return m_port_type_offsets[ static_cast<int>(type) ] + index;}

		template<PortType type>
		void portName(int index,const char* name)
			{
			jack_port_rename(m_handle,m_ports[portOffset<type>(index)],name);
			}

		void* r_cb_obj;
		Vtable m_vt;
		jack_client_t* m_handle;
		uint8_t m_port_type_offsets[PORT_TYPE_COUNT];
		std::vector<jack_port_t*> m_ports;
		std::vector<void*> m_buffers;
	};

AudioClient::AudioClient(const char* name,void* cb_obj,const Vtable& vt)
	{
	m_impl=new Impl(name,cb_obj,vt);
	}

AudioClient::~AudioClient()
	{delete m_impl;}

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
AudioClient::midiEvents(int port,int n_frames) noexcept
	{return m_impl->midiEvents(port,n_frames);}

AudioClient::MidiEvent AudioClient::MidiEventIterator::operator[](int k) noexcept
	{
	jack_midi_event_t e;
	jack_midi_event_get(&e,r_buffer,k);

	MidiEvent ret;
	ret.time_offset=e.time;
	for(int k=0;k<std::min(4,static_cast<int>(e.size));++k)
		{ret.message.bytes(k)=e.buffer[k];}
	return ret;
	}




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


AudioClient::Impl::Impl(const char* name,void* cb_obj,const Vtable& vt):AudioClient(*this)
	,r_cb_obj(cb_obj),m_vt(vt)
	{
	jack_status_t status;
 	m_handle=jack_client_open(name,JackNoStartServer,&status);
	if(m_handle==NULL)
		{throw Error(name," failed to connect to JACK.");}

	jack_set_process_callback(m_handle,[](uint32_t n_frames,void* obj)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		std::transform(self->m_ports.begin(),self->m_ports.end(),self->m_buffers.begin()
			,[n_frames](jack_port_t* port)
				{return jack_port_get_buffer(port,n_frames);});
		self->m_vt.process_callback(self->r_cb_obj,*self,n_frames);
		return 0;
		},this);

	m_port_type_offsets[0]=0;
	m_port_type_offsets[1]=m_port_type_offsets[0] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_IN);
	m_port_type_offsets[2]=m_port_type_offsets[1] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_OUT);
	m_port_type_offsets[3]=m_port_type_offsets[2] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_IN);
	ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_OUT);
	m_buffers.resize(m_ports.size());

	jack_activate(m_handle);
	}

AudioClient::Impl::~Impl()
	{
	std::for_each(m_ports.begin(),m_ports.end(),[this](jack_port_t* port)
		{jack_port_unregister(m_handle,port);});
	jack_deactivate(m_handle);
	jack_client_close(m_handle);
	m_impl=nullptr;
	}
