//@	{
//@	 "targets":[{"name":"audioclient.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"audioclient.o","rel":"implementation"}]
//@	}

#ifndef ANJA_AUDIOCLIENT_HPP
#define ANJA_AUDIOCLIENT_HPP

#include <utility>
#include <cstdint>

namespace Anja
	{
	class AudioClient
		{
		public:
			enum class PortType:int{MIDI_IN,MIDI_OUT,WAVE_IN,WAVE_OUT};

			template<class Callback>
			explicit AudioClient(const char* name,Callback& cb):
				AudioClient(name,&cb
				,Vtable
					{
					 [](void* cb_obj,AudioClient& self,int32_t n_frames)
						{reinterpret_cast<Callback*>(cb_obj)->process(self,n_frames);}
					,[](void* cb_obj,PortType type,int index)
						{return reinterpret_cast<Callback*>(cb_obj)->port(type,index);}
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
			struct Vtable
				{
				void (*process_callback)(void* cb_obj,AudioClient& self,int n_frames);
				const char* (*port_callback)(void* cb,PortType type,int index);
				};


			class Impl;
			Impl* m_impl;

			explicit AudioClient(Impl& impl):m_impl(&impl){}
			explicit AudioClient(const char* name,void* cb_obj,const Vtable& cb);
		};

	}

#endif // ANJA_ENGINE_HPP
