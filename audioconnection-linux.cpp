#ifdef __WAND__
target[name[audioconnection.o] type[object] platform[;GNU/Linux] dependency[jack;external]]
#endif

#include "audioconnection.h"
#include "units.h"
#include "ringbuffer.h"
#include <jack/jack.h>
#include <time.h>
#include <memory>
#include <cassert>

typedef std::unique_ptr<jack_client_t,decltype(&jack_client_close)>
	ConnectionHandle;

static ConnectionHandle connectionCreate(const char* name)
	{
	auto N_max=jack_client_name_size();
	ArraySimple<char> client_name(N_max);
	auto ptr_name=client_name.begin();
	while(N_max!=0 && *name!='\0')
		{
		auto ch_in=*name;
		*ptr_name=ch_in==':'?'_':ch_in;
		++ptr_name;
		++name;
		--N_max;
		}
	*ptr_name='\0';
	auto connection=jack_client_open(client_name.begin(),JackNoStartServer,NULL);
	if(connection==NULL)
		{throw "Could not connect to JACK";}
	return {connection,jack_client_close};
	}

static uint64_t clockGet(jack_nframes_t fs)
	{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW,&now);

	double t=1e-9*now.tv_nsec+now.tv_sec;
	return secondsToFrames(t,fs);
	}

class Port
	{
	public:
		Port(jack_client_t* client,const char* name
			,const char* type,unsigned long flags);

		~Port();

	private:
		jack_client_t* r_client;
		jack_port_t* m_port;
	};

Port::Port(jack_client_t* client,const char* name
	,const char* type,unsigned long flags):r_client(client)
	{
	m_port=jack_port_register(client,name,type,flags,0);
	if(m_port==NULL)
		{throw "Could not register another JACK port";}
	}

Port::~Port()
	{jack_port_unregister(r_client,m_port);}

class AudioConnectionJack:public AudioConnection
	{
	public:
		struct alignas(4) SlotEvent
			{
			uint16_t frames_delay;
			uint8_t slot;
			uint8_t flags;
			};

		AudioConnectionJack(const char* name);
		~AudioConnectionJack();
		void destroy()
			{delete this;}
		uint16_t timeOffsetGet() const
			{return clockGet(m_fs) - m_frames_processed;}

		void eventPost(uint8_t slot,uint8_t flags);

	private:
		ConnectionHandle m_connection;
		Port m_output;
		uint64_t m_frames_processed;
		RingBuffer<SlotEvent> m_event_queue;
		SlotEvent m_event_next;

		jack_nframes_t m_fs;

		SlotEvent* r_event_read;
		size_t offset_read;
		static int dataProcess(jack_nframes_t n_frames,void* audioconnectionjack);
	};

AudioConnection* AudioConnection::create(const char* name)
	{
	return new AudioConnectionJack(name);
	}

AudioConnectionJack::AudioConnectionJack(const char* name):
	m_connection{connectionCreate(name)}
	,m_output(m_connection.get(),"Audio output",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput)
	,m_event_queue(64)
	{
	m_fs=jack_get_sample_rate(m_connection.get());
	m_frames_processed=clockGet(m_fs);
	jack_set_process_callback(m_connection.get(),dataProcess,this);
	jack_activate(m_connection.get());
	}

AudioConnectionJack::~AudioConnectionJack()
	{
	}

void AudioConnectionJack::eventPost(uint8_t slot,uint8_t flags)
	{
	if(!m_event_queue.full())
		{
		m_event_queue.push_back({timeOffsetGet(),slot,uint8_t(flags|0x80)});
		}
	}

int AudioConnectionJack::dataProcess(jack_nframes_t n_frames,void* audioconnectionjack)
	{
	auto _this=reinterpret_cast<AudioConnectionJack*>(audioconnectionjack);
	jack_nframes_t n=0;
	auto& queue=_this->m_event_queue;
	SlotEvent event_next=_this->m_event_next;
	while(n!=n_frames)
		{
		if(event_next.flags&0x80 && n>=event_next.frames_delay)
			{
		//	Trigg event
			while(!queue.empty())
				{
				event_next=queue.pop_front();
				if(n>=event_next.frames_delay)
					{
				//Trigg event
					}
				else
					{break;}
				}
			}
		++n;
		}
	__sync_add_and_fetch(&_this->m_frames_processed,n_frames);
	event_next.frames_delay-=n_frames;
	_this->m_event_next=event_next;
	return 0;
	};
