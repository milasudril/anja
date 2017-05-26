//@	{
//@	 "targets":[{"name":"audioclient.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"audioclient.o","rel":"implementation"}]
//@	}

#ifndef ANJA_AUDIOCLIENT_HPP
#define ANJA_AUDIOCLIENT_HPP

#include <utility>

namespace Anja
	{
	class AudioClient
		{
		public:
			enum class PortType:int{MIDI_IN,MIDI_OUT,WAVE_IN,WAVE_OUT};

			struct PortInfo
				{
				const char* name;
				PortType type;
				};

			template<class Callback>
			explicit AudioClient(const char* name,Callback& cb):
				AudioClient(name,&cb,[](void* cb,int index,PortInfo& info)
					{
					return reinterpret_cast<Callback*>(cb)->port(index,info);
					})
				{}

			~AudioClient();

			AudioClient& operator=(AudioClient&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			AudioClient(AudioClient&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			AudioClient& midiInName(int index,const char* name);


			AudioClient& midiOutName(int index,const char* name);


			AudioClient& waveInName(int index,const char* name);


			AudioClient& waveOutName(int index,const char* name);

		private:
			typedef bool (*PortCallback)(void* cb,int index,PortInfo& info);

			class Impl;
			Impl* m_impl;

			explicit AudioClient(Impl& impl):m_impl(&impl){}
			explicit AudioClient(const char* name,void* cb_obj,PortCallback cb);
		};

	}

#endif // ANJA_ENGINE_HPP
