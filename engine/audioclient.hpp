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
					,[](void* cb_obj,AudioClient& self,PortType type,int index)
						{return reinterpret_cast<Callback*>(cb_obj)->portDisconnected(self,type,index);}
					})
				{}

			~AudioClient();

			AudioClient& activate();

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

			bool midiInConnected(int index) const noexcept;
			bool midiOutConnected(int index) const noexcept;
			bool waveInConnected(int index) const noexcept;
			bool waveOutConnected(int index) const noexcept;


			void waveOutConnect(int index,const char* target_port);
			void waveOutDisconnect(int index,const char* target_port);
			void midiOutConnect(int index,const char* target_port);
			void midiOutDisconnect(int index,const char* target_port);
			void waveInConnect(int index,const char* target_port);
			void waveInDisconnect(int index,const char* target_port);
			void midiInConnect(int index,const char* target_port);
			void midiInDisconnect(int index,const char* target_port);


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

			template<class Callback>
			bool midiInEnum(Callback&& cb)
				{
				return midiInEnum(&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool midiInConnectionsEnum(int index,Callback&& cb)
				{
				return midiInConnectionsEnum(index,&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool midiOutEnum(Callback&& cb)
				{
				return midiOutEnum(&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool midiOutConnectionsEnum(int index,Callback&& cb)
				{
				return midiOutConnectionsEnum(index,&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool waveInEnum(Callback&& cb)
				{
				return waveInEnum(&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool waveInConnectionsEnum(int index,Callback&& cb)
				{
				return waveInConnectionsEnum(index,&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}


			template<class Callback>
			bool waveOutEnum(Callback&& cb)
				{
				return waveOutEnum(&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

			template<class Callback>
			bool waveOutConnectionsEnum(int index,Callback&& cb)
				{
				return waveOutConnectionsEnum(index,&cb,[](void* cb_obj,AudioClient& client,const char* port_name)
					{return (*reinterpret_cast<Callback*>(cb_obj))(client,port_name);});
				}

		private:
			struct Vtable
				{
				void (*process_callback)(void* cb_obj,AudioClient& self,int n_frames);
				void (*buffersize_callback)(void* cb_obj,AudioClient& self,int n_frames);
				const char* (*port_callback)(void* cb,PortType type,int index);
				void (*port_connected)(void* cb,AudioClient& self,PortType type,int index);
				void (*port_disconnected)(void* cb,AudioClient& self,PortType type,int index);
				};

			Impl* m_impl;

			explicit AudioClient(Impl& impl):m_impl(&impl){}
			explicit AudioClient(const char* name,void* cb_obj,const Vtable& cb);


			typedef bool (*PortEnumCallback)(void* cb_obj,AudioClient& client
				,const char* port_name);

			bool midiInEnum(void* cb_obj,PortEnumCallback cb);
			bool midiOutEnum(void* cb_obj,PortEnumCallback cb);
			bool waveInEnum(void* cb_obj,PortEnumCallback cb);
			bool waveOutEnum(void* cb_obj,PortEnumCallback cb);

			bool waveOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
			bool waveInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
			bool midiInConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);
			bool midiOutConnectionsEnum(int index,void* cb_obj,PortEnumCallback cb);

		};

	}

#endif // ANJA_ENGINE_HPP
