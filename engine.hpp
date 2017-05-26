//@	{"targets":[{"name":"engine.hpp","type":"include"}]}

#ifndef ANJA_ENGINE_HPP
#define ANJA_ENGINE_HPP

#include "audioclient.hpp"
#include <cstdint>

namespace Anja
	{
	class Engine
		{
		public:
			union Message
				{
				uint32_t code;
				uint8_t bytes[4];
				};

			template<class PortInfo>
			Engine(const char* client_name,const PortInfo& ports):
				AudioClient(client_name,ports.midiIn(),ports.midiOut(),ports.waveIn(),ports.waveOut())
				{m_client.callback(*this);}

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

			Engine& messagePost();


		private:
			AudioClient m_client;
		};

	}

#endif // ANJA_ENGINE_HPP
