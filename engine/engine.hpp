//@	{
//@	"targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef ANJA_ENGINE_HPP
#define ANJA_ENGINE_HPP

#include "audioclient.hpp"
#include "../sessiondata/session.hpp"
#include "../common/thread.hpp"

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
			void run(TaskId id);

			Engine& messagePost(Message msg);

			bool running() const noexcept
				{return m_running;}


		private:
			const Session* r_session;
			volatile bool m_running;
			AudioClient m_client;
			Thread m_rec_thread;
		};

	}

#endif // ANJA_ENGINE_HPP
