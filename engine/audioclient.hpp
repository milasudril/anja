//@	{
//@	 "targets":[{"name":"audioclient.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"audioclient.o","rel":"implementation"}]
//@	}

#ifndef ANJA_AUDIOCLIENT_HPP
#define ANJA_AUDIOCLIENT_HPP

#include "../midiconstants/message.hpp"
#include <utility>
#include <cstdint>

namespace Anja
	{
	class AudioClient
		{
		private:
			class Impl;
		public:
			enum class PortType:int{MIDI_IN,MIDI_OUT,WAVE_IN,WAVE_OUT};
			static constexpr int PORT_TYPE_COUNT=4;

			template<class Callback>
			explicit AudioClient(const char* name,Callback& cb):
				AudioClient(name,&cb
				,Vtable
					{
					 [](void* cb_obj,AudioClient& self,int32_t n_frames)
						{reinterpret_cast<Callback*>(cb_obj)->process(self,n_frames);}
					,[](void* cb_obj,AudioClient& self,int32_t n_frames)
						{reinterpret_cast<Callback*>(cb_obj)->bufferSize(self,n_frames);}
					,[](void* cb_obj,PortType type,int index)
						{return reinterpret_cast<Callback*>(cb_obj)->port(type,index);}
					,[](void* cb_obj,AudioClient& self,PortType type,int index)
						{return reinterpret_cast<Callback*>(cb_obj)->portConnected(self,type,index);}
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

			int midiInCount() const noexcept;
			int midiOutCount() const noexcept;
			int waveInCount() const noexcept;
			int waveOutCount() const noexcept;

			class alignas(8) MidiEvent
				{
				public:
					MidiEvent()=default;

					MidiEvent(uint32_t time_offset,MIDI::Message msg) noexcept
						{timeOffset(time_offset).message(msg);}

					uint32_t timeOffset() const noexcept
						{return m_time_offset;}

					MidiEvent& timeOffset(uint32_t offset) noexcept
						{
						m_time_offset=offset;
						return *this;
						}

					MIDI::Message message() const noexcept
						{return m_message;}

					MidiEvent& message(MIDI::Message msg) noexcept
						{
						m_message=msg;
						return *this;
						}

					bool valid() const noexcept
						{return m_message.valid();}

					void clear() noexcept
						{m_message.clear();}

				private:
					uint32_t m_time_offset;
					MIDI::Message m_message;
				};

			class MidiEventIterator
				{
				public:
					MidiEvent operator*() noexcept
						{return (*this)[k];}

					MidiEventIterator& operator++() noexcept
						{
						++k;
						return *this;
						}

					MidiEvent operator[](int k) noexcept;

					bool operator==(const MidiEventIterator& i) const noexcept
						{return i.r_buffer==r_buffer && i.k==k;}

					bool operator!=(const MidiEventIterator& i) const noexcept
						{return !(*this==i);}

				private:
					friend class Impl;
					explicit MidiEventIterator(void* buffer,int init):r_buffer(buffer),k(init)
						{}

					void* r_buffer;
					int k;
				};

			class MidiMessageWriter
				{
				public:
					MidiMessageWriter& write(MIDI::Message msg,int frame) noexcept;

				private:
					friend class Impl;
					explicit MidiMessageWriter(void* buffer):r_buffer(buffer){}
					void* r_buffer;
				};

			std::pair<MidiEventIterator,MidiEventIterator> midiIn(int port,int n_frames) const noexcept;
			MidiMessageWriter midiOut(int port,int n_frames) const noexcept;
			const float* waveIn(int port,int n_frames) const noexcept;
			float* waveOut(int port,int n_frames) const noexcept;

			double sampleRate() const noexcept;

		private:
			struct Vtable
				{
				void (*process_callback)(void* cb_obj,AudioClient& self,int n_frames);
				void (*buffersize_callback)(void* cb_obj,AudioClient& self,int n_frames);
				const char* (*port_callback)(void* cb,PortType type,int index);
				void (*port_connected)(void* cb,AudioClient& self,PortType type,int index);
				};

			Impl* m_impl;

			explicit AudioClient(Impl& impl):m_impl(&impl){}
			explicit AudioClient(const char* name,void* cb_obj,const Vtable& cb);
		};

	}

#endif // ANJA_ENGINE_HPP
