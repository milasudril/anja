//@	{
//@	 "targets":[{"name":"voice.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"voice.o","rel":"implementation"}]
//@	}

#ifndef ANJA_VOICE_HPP
#define ANJA_VOICE_HPP

#include "../sessiondata/waveform.hpp"
#include "../common/units.hpp"

#include <cstdint>
#include <random>

namespace Anja
	{
	class Waveform;

	class Voice
		{
		public:
			Voice() noexcept:r_pos_current(nullptr),r_end(nullptr){}

			template<class Callback>
			explicit Voice(Waveform&& waveform,int channel,float velocity
				,int start_offset,Callback&,int id,int key)=delete;

			template<class Callback>
			explicit Voice(const Waveform& waveform,int channel,float velocity
				,int start_offset,Callback& cb,int id,int key):Voice(waveform,channel,velocity
				,start_offset,id,key)
				{
				r_cb_obj=&cb;
				m_vt.loop=[](void* cb_obj,Voice& self,int event_offset)
					{
					auto cb=reinterpret_cast<Callback*>(cb_obj);
					cb->loop(self,event_offset);
					};
				m_vt.playback_done=[](void* cb_obj,Voice& self,int event_offset)
					{
					auto cb=reinterpret_cast<Callback*>(cb_obj);
					cb->playbackDone(self,event_offset);
					};
				}

			void generate(float* buffer_out,int n_frames) noexcept;

			bool done() const noexcept
				{return r_pos_current==r_end || m_state==State::DONE;}

			Voice& stop(int offset) noexcept
				{
				if(m_flags&Waveform::SUSTAIN)
					{m_flags&=~Waveform::LOOP;}
				else
					{
					m_pos_offset=offset;
					m_state=State::END;
					}
				return *this;
				}

			Voice& flagsUnset(uint32_t flags_unset) noexcept
				{
				m_flags&=~flags_unset;
				return *this;
				}

			int channel() const noexcept
				{return m_channel;}

			uint16_t flags() const noexcept
				{return m_flags;}

			Voice& playFromLoopBegin() noexcept
				{
				r_pos_current=r_loop_begin;
				return *this;
				}

			template<class Rng>
			Voice& gainRandomize(Rng& r) noexcept
				{
				auto U=std::uniform_real_distribution<float>(-m_gain_random,m_gain_random);
				m_gain=dBToAmplitude(U(r) + m_gain_init);
				return *this;
				}

			int id() const noexcept
				{return m_id;}

			int key() const noexcept
				{return m_key;}

		private:
			explicit Voice(const Waveform& waveform,int channel,float velocity
				,int start_offset,int id,int key) noexcept;

			float m_velocity;
			float m_gain;
			float m_gain_random;
			const float* r_pos_current;

			const float* r_loop_begin;
			const float* r_loop_end;
			const float* r_end;
			int m_dir;
			int m_pos_offset;
			int m_channel;

			uint16_t m_flags;
			enum class State:uint16_t{BEGIN,RUNNING,END,DONE};
			State m_state;

			void* r_cb_obj;
			struct Vtable
				{
				void (*loop)(void* cb,Voice& src,int event_offset);
				void (*playback_done)(void* cb,Voice& src,int event_offset);
				} m_vt;
			float m_gain_init;
			int m_key;
			int m_id;
		};
	}

#endif // ANJA_VOICE_HPP
