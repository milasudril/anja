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
		typedef jack_default_audio_sample_t SampleType;

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


	private:
		template<PortType type>
		int portOffset(int index) const noexcept
			{return m_port_type_offsets[ static_cast<int>(type) ] + index;}

		template<PortType type>
		void portName(int index,const char* name)
			{
			jack_port_rename(m_handle,m_ports[portOffset<type>(index)],name);
			}

		void* r_cb_obj;
		Vtable m_vt;
		jack_client_t* m_handle;
		double m_sample_rate;
		uint8_t m_port_type_offsets[PORT_TYPE_COUNT];
		std::vector<jack_port_t*> m_ports;
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
AudioClient::midiIn(int port,int n_frames) const noexcept
	{return m_impl->midiIn(port,n_frames);}

AudioClient::MidiEvent AudioClient::MidiEventIterator::operator[](int k) noexcept
	{
	jack_midi_event_t e;
	jack_midi_event_get(&e,r_buffer,k);

	MidiEvent ret;
	ret.time_offset=e.time;
	switch(e.size)
		{
		case 3:
			ret.message=MIDI::Message(e.buffer[0],e.buffer[1],e.buffer[2]);
			return ret;
		case 2:
			ret.message=MIDI::Message(e.buffer[0],e.buffer[1],0);
			return ret;
		case 1:
			ret.message=MIDI::Message(e.buffer[0],0,0);
			return ret;
		}
	return ret;
	}


AudioClient::MidiMessageWriter AudioClient::midiOut(int port,int n_frames) const noexcept
	{return m_impl->midiOut(port,n_frames);}


AudioClient::MidiMessageWriter& AudioClient::MidiMessageWriter::write(MIDI::Message msg,int frame) noexcept
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
		{
		m_impl=nullptr;
		throw Error("The client \"",name,"\" failed to connect to JACK. Check if the server is running.");
		}

	jack_set_process_callback(m_handle,[](uint32_t n_frames,void* obj)
		{
		auto self=reinterpret_cast<Impl*>(obj);
		self->m_vt.process_callback(self->r_cb_obj,*self,n_frames);
		return 0;
		},this);

	m_port_type_offsets[0]=0;
	m_port_type_offsets[1]=m_port_type_offsets[0] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_IN);
	m_port_type_offsets[2]=m_port_type_offsets[1] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::MIDI_OUT);
	m_port_type_offsets[3]=m_port_type_offsets[2] + ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_IN);
	ports_create(m_handle,vt,cb_obj,m_ports,PortType::WAVE_OUT);
	m_sample_rate=jack_get_sample_rate(m_handle);
	jack_activate(m_handle);
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
