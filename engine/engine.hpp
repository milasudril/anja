//@	{
//@	"targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef ANJA_ENGINE_HPP
#define ANJA_ENGINE_HPP

#include "audioclient.hpp"
#include "voice.hpp"
#include "../sessiondata/session.hpp"
#include "../common/thread.hpp"
#include "../common/readysignal.hpp"
#include "../common/ringbuffer.hpp"
#include "../common/clock.hpp"
#include "../common/idgenerator.hpp"
#include "../pcg-cpp/include/pcg_random.hpp"

#include <cstdint>
#include <utility>

namespace Anja
	{
	class Message;
	class Engine
		{
		public:
			Engine(Session&&)=delete;

			explicit Engine(const Session& session);

			~Engine();

			Engine& midiInName(int index,const char* name)
				{
				m_client.midiInName(index,name);
				return *this;
				}

			Engine& midiOutName(int index,const char* name)
				{
				m_client.midiOutName(index,name);
				return *this;
				}

			Engine& waveInName(int index,const char* name)
				{
				m_client.waveInName(index,name);
				return *this;
				}

			Engine& waveOutName(int index,const char* name)
				{
				m_client.waveOutName(index,name);
				return *this;
				}

			void process(AudioClient& client,int n_frames) noexcept;
			void bufferSize(AudioClient& client,int n_frames) noexcept;
			const char* port(AudioClient::PortType type,int index) const noexcept;
			void portConnected(AudioClient& client,AudioClient::PortType type,int index) noexcept;


			enum class TaskId:int{RECORD};

			template<TaskId id> void run();

			Engine& messagePost(MIDI::Message msg) noexcept
				{
				if(!m_ui_events.full())
					{m_ui_events.push_back(AudioClient::MidiEvent{static_cast<uint32_t>(now_ms() + 1 - m_time_init),msg});}
				else
					{printf("Queue is full\n");}
				return *this;
				}

			Engine& channelGain(int channel,float value) noexcept
				{
				assert(channel>=0 && channel<16);
				return messagePost(MIDI::Message(MIDI::ControlCodes::CHANNEL_VOLUME,channel,dB_to_MIDI_val(value)));
				}

		private:
			const Session* r_session;
			volatile bool m_running;
			RingBuffer<AudioClient::MidiEvent,volatile uint32_t> m_ui_events;
			uint64_t m_time_init;
			AudioClient::MidiEvent m_event_last;
			ArraySimple<Voice> m_voices;
			typedef int VoiceIndex;
			ArraySimple<VoiceIndex> m_key_to_voice_index;
			IdGenerator<RingBuffer<VoiceIndex,uint32_t>> m_voices_alloc;
			ArraySimple<float> m_channel_buffers;
			ArraySimple<float> m_channel_gain;
			pcg32 m_rng;

			AudioClient m_client;
			ReadySignal m_ready;
			Thread m_rec_thread;

			void process(MIDI::Message msg,int offset) noexcept;

			static float dB_to_MIDI_val(float val) noexcept
				{return 127.0f*(val + 72.0f)/78.0f;}

			static float MIDI_val_to_dB(float val) noexcept
				{return 78.0f*val/127.0f - 72.0f;}

			int indexAudition(const AudioClient& client) const noexcept;
			int indexMaster(const AudioClient& client) const noexcept;

		};

	template<>
	void Engine::run<Engine::TaskId::RECORD>();
	}

#endif // ANJA_ENGINE_HPP
