#ifdef __WAND__
target[name[audioengineanja.h] type[include]]
dependency[audioengineanja.o]
#endif

#ifndef AUDIOENGINEANJA_H
#define AUDIOENGINEANJA_H

#include "audioconnection.h"
#include "ringbuffer.h"

class Session;

class AudioEngineAnja:public AudioConnection::AudioEngine
	{
	public:
		AudioEngineAnja();

		void audioProcess(AudioConnection& source,unsigned int n_frames) noexcept;
		void buffersAllocate(AudioConnection& source,unsigned int n_frames);
		void buffersFree();

		void eventMIDIPost(uint8_t status,uint8_t value_0
			,uint8_t value_1);

		void eventChannelVolumePost(float value);

	private:
		struct alignas(16) Event
			{
			uint64_t time;
			uint8_t status_word[4];
			float value;
			};

		uint64_t m_time_start;
		RingBuffer<Event> m_event_queue;
	};

#endif
