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
#include "../common/arraymultisimple.hpp"
#include "../common/clock.hpp"
#include "../common/idgenerator.hpp"
#include "../common/vec4d.hpp"
#include "../pcg-cpp/include/pcg_random.hpp"

#include <cstdint>
#include <utility>

namespace Anja
	{
	class Message;
	class Engine
		{
		public:
			template<class Callback>
			explicit Engine(Session& session,Callback& cb):Engine(session)
				{
				callback(cb);
				m_client.activate();
				init_notify();
				}

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

			bool midiInConnected(int index) const noexcept
				{return m_client.midiInConnected(index);}

			bool midiOutConnected(int index) const noexcept
				{return m_client.midiOutConnected(index);}

			bool waveInConnected(int index) const noexcept
				{return m_client.waveInConnected(index);}

			bool waveOutConnected(int index) const noexcept
				{return m_client.waveOutConnected(index);}


			int midiInCount() const noexcept
				{return m_client.midiInCount();}

			int midiOutCount() const noexcept
				{return m_client.midiOutCount();}

			int waveInCount() const noexcept
				{return m_client.waveInCount();}

			int waveOutCount() const noexcept
				{return m_client.waveOutCount();}


			void waveOutConnect(int index,const char* target_port)
				{m_client.waveOutConnect(index,target_port);}

			void waveOutDisconnect(int index,const char* target_port)
				{m_client.waveOutDisconnect(index,target_port);}

			void midiOutConnect(int index,const char* target_port)
				{m_client.midiOutConnect(index,target_port);}

			void midiOutDisconnect(int index,const char* target_port)
				{m_client.midiOutDisconnect(index,target_port);}


			void waveInConnect(int index,const char* target_port)
				{m_client.waveInConnect(index,target_port);}

			void waveInDisconnect(int index,const char* target_port)
				{m_client.waveInDisconnect(index,target_port);}

			void midiInConnect(int index,const char* target_port)
				{m_client.midiInConnect(index,target_port);}

			void midiInDisconnect(int index,const char* target_port)
				{m_client.midiInDisconnect(index,target_port);}



			bool muted(int index) const noexcept
				{return !(m_ch_state&(1<<index));}


			void process(AudioClient& client,int n_frames) noexcept;
			void bufferSize(AudioClient& client,int n_frames);
			const char* port(AudioClient::PortType type,int index) const noexcept;
			void portConnected(AudioClient& client,AudioClient::PortType type,int index);
			void portDisconnected(AudioClient& client,AudioClient::PortType type,int index);
			void loop(Voice& voice,int event_offset) noexcept;
			void playbackDone(Voice& voice,int event_offset) noexcept;


			enum class TaskId:int{RECORD};

			template<TaskId id> void run();

			Engine& messagePost(MIDIConstants::Message msg) noexcept
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
				return messagePost(MIDIConstants::Message(MIDIConstants::ControlCodes::CHANNEL_VOLUME,channel,dB_to_MIDI_val(value)));
				}

			Engine& fadeOut(int channel,float time) noexcept
				{
				assert(channel>=0 && channel<16);
				return messagePost(MIDIConstants::Message(FADE_OUT,channel,sec_to_MIDI_val(time)));
				}


			Engine& fadeIn(int channel,float time) noexcept
				{
				assert(channel>=0 && channel<16);
				return messagePost(MIDIConstants::Message(FADE_IN,channel,sec_to_MIDI_val(time)));
				}

			Engine& recordStart(int slot) noexcept
				{
				assert(slot>=0 && slot<128);
				return messagePost(MIDIConstants::Message(RECORD_START,0,slot));
				}

			Engine& recordStop(int slot) noexcept
				{
				assert(slot>=0 && slot<128);
				return messagePost(MIDIConstants::Message{RECORD_STOP,0,slot});
				}

			double sampleRate() const noexcept
				{return m_client.sampleRate();}


			template<class Callback>
			bool midiInEnum(Callback&& cb)
				{return m_client.midiInEnum(std::move(cb));}

			template<class Callback>
			bool midiInConnectionsEnum(int index,Callback&& cb)
				{return m_client.midiInConnectionsEnum(index,std::move(cb));}


			template<class Callback>
			bool midiOutEnum(Callback&& cb)
				{return m_client.midiOutEnum(std::move(cb));}

			template<class Callback>
			bool midiOutConnectionsEnum(int index,Callback&& cb)
				{return m_client.midiOutConnectionsEnum(index,std::move(cb));}


			template<class Callback>
			bool waveInEnum(Callback&& cb)
				{return m_client.waveInEnum(std::move(cb));}

			template<class Callback>
			bool waveInConnectionsEnum(int index,Callback&& cb)
				{return m_client.waveInConnectionsEnum(index,std::move(cb));}


			template<class Callback>
			bool waveOutEnum(Callback&& cb)
				{return m_client.waveOutEnum(std::move(cb));}

			template<class Callback>
			bool waveOutConnectionsEnum(int index,Callback&& cb)
				{return m_client.waveOutConnectionsEnum(index,std::move(cb));}


			static constexpr auto FADE_OUT=MIDIConstants::ControlCodes::GENERAL_PURPOSE_1;
			static constexpr auto FADE_IN=MIDIConstants::ControlCodes::GENERAL_PURPOSE_2;
			static constexpr auto RECORD_START=MIDIConstants::ControlCodes::GENERAL_PURPOSE_3;
			static constexpr auto RECORD_STOP=MIDIConstants::ControlCodes::GENERAL_PURPOSE_4;

		private:

			Session* r_session;
			volatile bool m_running;
			RingBuffer<AudioClient::MidiEvent,volatile uint32_t> m_ui_events;
			uint64_t m_time_init;
			AudioClient::MidiEvent m_event_last;
			ArraySimple<Voice> m_voices;
			typedef int VoiceIndex;
			ArraySimple<VoiceIndex> m_key_to_voice_index;
			IdGenerator<RingBuffer<VoiceIndex,uint32_t>> m_voices_alloc;
			ArraySimple<float> m_channel_buffers;

#ifdef __AVX__
			typedef Vec4dAVX Vec4d;
#else
			typedef Vec4dSSE2 Vec4d;
#endif
			ArrayMultiSimple<float,std::pair<Vec4d,Vec4d>> m_channel_gain;
			ArrayMultiSimple<float,float> m_rec_buffers;
			int m_rec_write_offset;
			uint16_t m_ch_state;
			pcg32 m_rng;

			enum RecAction:int16_t{NOP,BEGIN,END};

			union RecordMessage
				{
				public:
					RecordMessage():m_storage(0){}

					RecordMessage(volatile const RecordMessage& x) __attribute__((always_inline)):
						m_storage(x.m_storage){}

					void operator=(const RecordMessage& x) volatile __attribute__((always_inline))
						{
						m_storage=x.m_storage;
						}

					RecordMessage(RecAction action,int16_t shortval,int32_t longval)
						{
						m_vals.action=action;
						m_vals.shortval=shortval;
						m_vals.longval=longval;
						}

					RecAction action() const noexcept
						{return m_vals.action;}

					int16_t shortval() const noexcept
						{return m_vals.shortval;}

					int32_t longval() const noexcept
						{return m_vals.longval;}

				private:
					int64_t m_storage;
					struct Values
						{
						RecAction action;
						int16_t shortval;
						int32_t longval;
						} m_vals;
				};
			volatile RecordMessage m_rec_message;
			volatile int m_rec_length;
			RecordMessage m_rec_message_in;
			AudioClient m_client;
			ReadySignal m_ready;
			Thread m_rec_thread;

			void process(MIDIConstants::Message msg,int offset,double fs) noexcept;

			static float dB_to_MIDI_val(float val) noexcept
				{return 127.0f*(val + 72.0f)/78.0f;}

			static float MIDI_val_to_dB(float val) noexcept
				{return 78.0f*val/127.0f - 72.0f;}

			static float sec_to_MIDI_val(float time) noexcept
				{return 127.0f*std::log10(1.0e3f*time)/4.0f;}

			static float MIDI_val_to_sec(float val) noexcept
				{return 1e-3f*std::pow(10.0f,4.0f*val/127.0f);}

			static double sec_to_decay_factor(double time,double fs) noexcept
				{return std::pow(10.0,-4.0/(2.0*time*fs));}


			int indexAudition(const AudioClient& client) const noexcept;
			int indexMaster(const AudioClient& client) const noexcept;

			struct Vtable
				{
				void (*muted)(void* cb_obj,Engine& engine,int channel);
				void (*unmuted)(void* cb_obj,Engine& engine,int channel);
				void (*record_done)(void* cb_obj,Engine& engine,int slot);
				void (*port_connected)(void* cb_obj,Engine& engine,AudioClient::PortType type,int index);
				void (*port_disconnected)(void* cb_obj,Engine& engine,AudioClient::PortType type,int index);
				} m_vt;
			void* r_cb_obj;

			explicit Engine(Session& session);

			template<class Callback>
			Engine& callback(Callback& cb) noexcept
				{
				m_vt.muted=[](void* cb_obj,Engine& engine,int channel)
					{reinterpret_cast<Callback*>(cb_obj)->muted(engine,channel);};
				m_vt.unmuted=[](void* cb_obj,Engine& engine,int channel)
					{reinterpret_cast<Callback*>(cb_obj)->unmuted(engine,channel);};
				m_vt.record_done=[](void* cb_obj,Engine& engine,int slot)
					{reinterpret_cast<Callback*>(cb_obj)->recordDone(engine,slot);};
				m_vt.port_connected=[](void* cb_obj,Engine& engine,AudioClient::PortType type,int index)
					{reinterpret_cast<Callback*>(cb_obj)->portConnected(engine,type,index);};
				m_vt.port_disconnected=[](void* cb_obj,Engine& engine,AudioClient::PortType type,int index)
					{reinterpret_cast<Callback*>(cb_obj)->portDisconnected(engine,type,index);};
				r_cb_obj=&cb;
				return *this;
				}

			void init_notify();
		};

	template<>
	void Engine::run<Engine::TaskId::RECORD>();
	}

#endif // ANJA_ENGINE_HPP
