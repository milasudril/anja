#ifdef __WAND__
target[name[audioengineanja.h] type[include]]
dependency[audioengineanja.o]
#endif

#ifndef AUDIOENGINEANJA_H
#define AUDIOENGINEANJA_H

#include "audioconnection.h"
#include "ringbuffer.h"
#include "framework/array_simple.h"
#include "framework/array_fixed.h"
#include "framework/randomgeneratorimpl.h"
#include "framework/thread.h"

class Session;
class PlaybackRange;
class RecordBuffers;
class Wavetable;
class Waveform;
class Event;

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

		void masterGainSet(float gain) noexcept
			{m_master_gain_in=gain;}

		float masterGainGet() const noexcept
			{return m_master_gain_in;}

		void multioutputSet(bool state) noexcept
			{m_multioutput=state;}

		void waitForFrame() noexcept;

		void waitForRecordComplete() noexcept;

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

		double m_fader_filter_factor;
		struct ChannelState
			{
			double gain_in_old; /**<Old  fader value*/
			double gain_in_current; /**<Value set by channel fader*/

			double gain_out; /**<Output gain after fade-in/out*/
			double fade_factor; /**<Fade-in/out factor */
			};
		ArrayFixed<ChannelState,16> m_channels;

		ArraySimple<float> m_buffer_temp;
		ArraySimple<float> m_buffers_out;
		RecordBuffers* m_rec_buffers;
		Thread* m_rec_thread;
		class RecordTask:public Thread::Task
			{
			public:
				RecordTask();

				~RecordTask();

				unsigned int run();

				RecordTask& recordBuffersSet(RecordBuffers* rec_buffers) noexcept
					{
					r_rec_buffers=rec_buffers;
					return *this;
					}

				RecordTask& stop() noexcept
					{
					m_flags|=STOPPED;
					return *this;
					}

				RecordTask& stopReset() noexcept
					{
					m_flags&=~STOPPED;
					return *this;
					}

				bool stopped() noexcept
					{return m_flags&STOPPED;}

				void enable() noexcept
					{m_flags|=ENABLED;}

				bool enabled() const noexcept
					{return m_flags&ENABLED;}

				RecordTask& disable() noexcept;

				RecordTask& waveformSet(Waveform& waveform) noexcept;

			private:
				static constexpr unsigned int ENABLED=0x1;
				static constexpr unsigned int STOPPED=0x2;

				RecordBuffers* r_rec_buffers;
				Waveform* volatile r_waveform;
				volatile unsigned int m_flags;
				::Event* m_ready;
			} m_record_task;

		double m_master_gain_out;
		double m_master_gain_in;
		bool m_multioutput;
		::Event* m_ready;
		::Event* m_record_complete;

		static AudioEngineAnja::Event eventConvert(const AudioConnection::MIDIEvent& e) noexcept;
		void eventWrite( AudioConnection& output_connection
			,AudioConnection::MIDIBufferOutputHandle midi_out
			,const Event& event,unsigned int time_offset) noexcept;
		void eventsRead(AudioConnection& source,unsigned int n_frames) noexcept;

		void eventProcess(const Event& event,unsigned int time_offset) noexcept;
		void eventControlProcess(const Event& event,unsigned int time_offset) noexcept;

		void eventsUIFetch(AudioConnection& source,unsigned int n_frames) noexcept;
		void voicesRender(unsigned int n_frames) noexcept;
		void channelsMix(AudioConnection& source,unsigned int n_frames) noexcept;
		void masterGainAdjust(AudioConnection& source,unsigned int n_frames) noexcept;
		void voicesStop() noexcept;
		void capture(AudioConnection& source,unsigned int n_frames) noexcept;

		void reset();
	};

#endif
