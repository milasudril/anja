#ifdef __WAND__
target[name[audioengineanja.h] type[include]]
dependency[audioengineanja.o]
#endif

#ifndef AUDIOENGINEANJA_H
#define AUDIOENGINEANJA_H

#include "audioconnection.h"
#include "ringbuffer.h"
#include "framework/array_simple.h"
#include "framework/randomgeneratorimpl.h"

class Session;
class PlaybackRange;
class Wavetable;

class AudioEngineAnja:public AudioConnection::AudioEngine
	{
	public:
		AudioEngineAnja(Wavetable& waveforms);
		~AudioEngineAnja();

		void onActivate(AudioConnection& source);
		void onDeactivate(AudioConnection& source);

		void audioProcess(AudioConnection& source,unsigned int n_frames) noexcept;
		void buffersAllocate(AudioConnection& source,unsigned int n_frames);

		void eventPost(uint8_t status,uint8_t value_0,uint8_t value_1) noexcept;
		void eventPost(uint8_t status,uint8_t value_0,float value_1) noexcept;

	private:
		struct alignas(16) Event
			{
			static constexpr uint8_t VALUE_1_FLOAT=1;

			uint64_t delay;
			uint8_t status_word[4];
			float value;
			};

		Wavetable* r_waveforms;

		double m_sample_rate;
		uint64_t m_time_start;
		uint64_t m_now;

	//	Force vectorized instructions to pop elements form the event queue
		typedef float QueueVector __attribute__((vector_size(16)));
		union QueueElement
			{
			Event event;
			QueueVector vector;
			};
		RingBuffer<QueueVector> m_event_queue;

		Event m_event_next;

		uint8_t m_voice_current;
		RandomGeneratorImpl<std::mt19937> m_randgen;
		ArraySimple<PlaybackRange> m_source_buffers;
		ArraySimple<uint8_t> r_source_buffers;
		ArraySimple<float> m_voice_channels;

		void eventProcess(const Event& event,unsigned int time_offset);
	};

#endif
