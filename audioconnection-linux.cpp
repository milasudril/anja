#ifdef __WAND__
target[name[audioconnection.o] type[object] platform[;GNU/Linux] dependency[jack;external]]
#endif

#include "audioconnection.h"
#include "units.h"
#include "ringbuffer.h"
#include "playbackrange.h"
#include "session.h"
#include "framework/array_fixed.h"
#include <jack/jack.h>
#include <time.h>
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

		void* bufferGet(jack_nframes_t n_frames) const
			{return jack_port_get_buffer(m_port,n_frames);}

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
		struct SlotEvent
			{
			SlotEvent(const SlotEvent&)=default;
			SlotEvent& operator=(const SlotEvent&)=default;
			SlotEvent():delay(0){}
			SlotEvent(const Waveform& waveform,uint64_t d):
				data(waveform),delay(d){}
			PlaybackRange data;
			uint64_t delay;
			};

		AudioConnectionJack(const char* name);
		~AudioConnectionJack();
		void destroy()
			{delete this;}
		uint64_t timeOffsetGet() const
			{return clockGet(m_fs) - m_time_start;}

		void eventPost(Session& session,uint8_t slot,uint8_t flags);

	private:
		ConnectionHandle m_connection;
		Port m_output;
		uint64_t m_time_start;
		uint64_t m_now;
		RingBuffer<SlotEvent> m_event_queue;
		SlotEvent m_event_next;
		ArraySimple<SlotEvent> m_buffers;

		jack_nframes_t m_fs;

		std::mt19937 m_randgen;

		static int dataProcess(jack_nframes_t n_frames,void* audioconnectionjack);
		static int bufferAllocate(jack_nframes_t nframes,void* audioconnectionjack);
	};

AudioConnection* AudioConnection::create(const char* name)
	{
	return new AudioConnectionJack(name);
	}

AudioConnectionJack::AudioConnectionJack(const char* name):
	m_connection{connectionCreate(name)}
	,m_output(m_connection.get(),"Audio output",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput)
	,m_event_queue(64)
	,m_buffers(32)
	{
	m_fs=jack_get_sample_rate(m_connection.get());
	m_time_start=clockGet(m_fs);
	m_now=m_time_start;
	jack_set_process_callback(m_connection.get(),dataProcess,this);
	jack_set_buffer_size_callback(m_connection.get(),bufferAllocate,this);
	jack_activate(m_connection.get());
	}

AudioConnectionJack::~AudioConnectionJack()
	{
	}

void AudioConnectionJack::eventPost(Session& session,uint8_t slot,uint8_t flags)
	{
	if(!m_event_queue.full() && flags==0x90)
		{
		m_event_queue.push_back(
			{session.waveformGet(slot),timeOffsetGet()});
		}
	}

int AudioConnectionJack::bufferAllocate(jack_nframes_t n_frames,void* audioconnectionjack)
	{
	printf("Hello\n");
	return 0;
	}

int AudioConnectionJack::dataProcess(jack_nframes_t n_frames,void* audioconnectionjack)
	{
	auto _this=reinterpret_cast<AudioConnectionJack*>(audioconnectionjack);
	auto& queue=_this->m_event_queue;
	SlotEvent event_next=_this->m_event_next;
	auto now=_this->m_now;
	auto buffer=reinterpret_cast<float*>(_this->m_output.bufferGet(n_frames));
	auto buffer_0=buffer;
	auto n_frames_in=n_frames;
	std::uniform_real_distribution<float> U(-1,1);
	while(n_frames!=0)
		{
		if(event_next.data.valid() && now>=event_next.delay)
			{
		//	Trigg event
		//	printf("Got event %u\n",__LINE__);
			event_next.delay=buffer-buffer_0;
			_this->m_buffers[0]=event_next;
			event_next.data.reset();
			}

		while(!queue.empty())
			{
			event_next=queue.pop_front();
			if(now>=event_next.delay && event_next.data.valid())
				{
			//	printf("Got event %u\n",__LINE__);
				event_next.delay=buffer-buffer_0;
				_this->m_buffers[0]=event_next;
				event_next.data.reset();
				}
			else
				{break;}
			}
		++now;
		++buffer;
		--n_frames;
		}

	memset(buffer_0,0,n_frames_in*sizeof(float));
	if(_this->m_buffers[0].data.valid())
		{
		auto delay=_this->m_buffers[0].delay;
		_this->m_buffers[0].data.outputBufferGenerate(
			buffer_0+delay,n_frames_in-delay,_this->m_fs);
		_this->m_buffers[0].delay=0;
		}

	_this->m_now=now;
	_this->m_event_next=event_next;
	return 0;
	};
