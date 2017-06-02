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

			void process(AudioClient& client,int32_t n_frames) noexcept;
			const char* port(AudioClient::PortType type,int index) const noexcept;


			enum class TaskId:int{RECORD};

			template<TaskId id> void run();

			Engine& messagePost(MIDI::Message msg) noexcept
				{
				m_ui_events.push_back({static_cast<uint32_t>(now_ms() - m_time_init),msg});
				return *this;
				}

		private:
			const Session* r_session;
			volatile bool m_running;
			RingBuffer<AudioClient::MidiEvent> m_ui_events;
			uint64_t m_time_init;
			AudioClient::MidiEvent m_event_last;
			ArraySimple<Voice> m_voices;
			IdGenerator<RingBuffer<uint8_t>> m_voices_alloc;

			AudioClient m_client;
			ReadySignal m_ready;
			Thread m_rec_thread;

		};

	template<>
	void Engine::run<Engine::TaskId::RECORD>();
	}

#endif // ANJA_ENGINE_HPP
