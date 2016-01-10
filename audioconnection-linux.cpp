#ifdef __WAND__
target[name[audioconnection.o] type[object] platform[;GNU/Linux] dependency[jack;external]]
#endif

#include "audioconnection.h"
#include "units.h"
#include "ringbuffer.h"
#include "playbackrange.h"
#include "framework/array_fixed.h"
#include "framework/arraydynamicshort.h"

#include <jack/jack.h>

#include <memory>
#include <cassert>
#include <cstring>
#include <random>

typedef std::unique_ptr<jack_client_t,decltype(&jack_client_close)>
	ConnectionHandle;

static ConnectionHandle connectionCreate(const char* name)
	{
	auto N_max=jack_client_name_size();
	ArraySimple<char> client_name(N_max);
	auto ptr_name=client_name.begin();
	memset(ptr_name,0,N_max*sizeof(char));
	while(N_max!=0 && *name!='\0')
		{
		auto ch_in=*name;
		*ptr_name=ch_in==':'?'_':ch_in;
		++ptr_name;
		++name;
		--N_max;
		}
	*ptr_name='\0';
	jack_status_t status;
	auto connection=jack_client_open(client_name.begin(),JackNoStartServer
		,&status,"default");
	if(connection==NULL)
		{throw "Could not connect to JACK";}
	return {connection,jack_client_close};
	}

class Port
	{
	public:
		Port():r_client(nullptr),m_port(nullptr)
			{}

		Port(const Port& port)=delete;

		Port& operator=(const Port& port)=delete;

		Port(Port&& port):r_client(port.r_client),m_port(port.m_port)
			{port.m_port=nullptr;}

		Port& operator=(Port&& port)
			{
			std::swap(port.m_port,m_port);
			std::swap(port.r_client,r_client);
			return *this;
			}

		Port(jack_client_t* client,const char* name
			,const char* type,unsigned long flags):r_client(client)
			{
			m_port=jack_port_register(client,name,type,flags,0);
			if(m_port==NULL)
				{throw "Could not register a new JACK port";}
			}

		~Port()
			{
			if(m_port!=nullptr)
				{jack_port_unregister(r_client,m_port);}
			}

		void* bufferGet(jack_nframes_t n_frames) const
			{return jack_port_get_buffer(m_port,n_frames);}

	private:
		jack_client_t* r_client;
		jack_port_t* m_port;
	};



class AudioConnectionJack:public AudioConnection
	{
	public:
		AudioConnectionJack(const char* name,AudioEngine& engine);
		~AudioConnectionJack();

		void activate()
			{jack_activate(m_connection.get());}

		void deactivate()
			{jack_deactivate(m_connection.get());}

		const float* audioBufferInputGet(unsigned int index,unsigned int n_frames) const
			{return static_cast<const float*>(m_audio_in_ports[index].bufferGet(n_frames));}

		AudioConnection& audioPortInputAdd(const char* name)
			{
			m_audio_in_ports.append(
				{
				 m_connection.get()
				,name
				,JACK_DEFAULT_AUDIO_TYPE
				,JackPortIsInput
				});
			return *this;
			}

		unsigned int audioPortsInputCount() const
			{return m_audio_in_ports.length();}

		float* audioBufferOutputGet(unsigned int index,unsigned int n_frames) const
			{return static_cast<float*>(m_audio_out_ports[index].bufferGet(n_frames));}

		AudioConnection& audioPortOutputAdd(const char* name)
			{
			m_audio_out_ports.append(
				{
				 m_connection.get()
				,name
				,JACK_DEFAULT_AUDIO_TYPE
				,JackPortIsOutput
				});
			return *this;
			}

		unsigned int audioPortsOutputCount() const
			{return m_audio_out_ports.length();}



		void destroy()
			{delete this;}

		double sampleRateGet() const
			{return m_fs;}

	private:
		ConnectionHandle m_connection;
		AudioEngine* r_engine;
		Port m_output;
		ArrayDynamicShort<Port> m_audio_in_ports;
		ArrayDynamicShort<Port> m_audio_out_ports;
		jack_nframes_t m_fs;
		static int dataProcess(jack_nframes_t n_frames,void* audioconnectionjack);
		static int bufferAllocate(jack_nframes_t nframes,void* audioconnectionjack);
	};

AudioConnection* AudioConnection::create(const char* name,AudioEngine& engine)
	{
	return new AudioConnectionJack(name,engine);
	}

AudioConnectionJack::AudioConnectionJack(const char* name,AudioEngine& engine):
	m_connection{connectionCreate(name)},r_engine(&engine)
	{
	jack_set_process_callback(m_connection.get(),dataProcess,this);
	jack_set_buffer_size_callback(m_connection.get(),bufferAllocate,this);
	r_engine->onActivate(*this);
	jack_activate(m_connection.get());
	}

AudioConnectionJack::~AudioConnectionJack()
	{
	r_engine->onDeactivate(*this);
	jack_deactivate(m_connection.get());
	}

int AudioConnectionJack::bufferAllocate(jack_nframes_t n_frames,void* audioconnectionjack)
	{
	auto _this=reinterpret_cast<AudioConnectionJack*>(audioconnectionjack);
	_this->r_engine->buffersAllocate(*_this,n_frames);
	return 0;
	}

int AudioConnectionJack::dataProcess(jack_nframes_t n_frames,void* audioconnectionjack)
	{
	auto _this=reinterpret_cast<AudioConnectionJack*>(audioconnectionjack);
	_this->r_engine->audioProcess(*_this,n_frames);
	return 0;
	}
