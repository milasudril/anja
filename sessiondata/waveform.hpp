//@	{
//@	 "targets":[{"name":"waveform.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveform.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORM_HPP
#define ANJA_WAVEFORM_HPP

#include "../common/arraydynamicshort.hpp"

#include <algorithm>
#include <cstdint>

namespace Anja
	{
	class Waveform
		{
		public:
			Waveform() noexcept:m_offset_begin(0),m_offset_end(0),m_channel(0)
				,m_gain(0.0f),m_gain_random(0.0f),m_fs(0),m_flags(0)
				{}

			explicit Waveform(const float* buffer,uint32_t buffer_size,float fs) noexcept:
				m_data(buffer,buffer_size),m_offset_begin(0),m_offset_end(buffer_size)
				,m_gain(0.0f),m_gain_random(0.0f),m_fs(fs),m_flags(READONLY)
				{}

			void fileLoad(const char* filename);

			void fileSave(const char* filename);

			const float* begin() const noexcept
				{return m_data.begin() + m_offset_begin;}

			const float* beginFull() const noexcept
				{return m_data.begin();}

			const float* end() const noexcept
				{return m_data.begin() + m_offset_end;}

			const float* endFull() const noexcept
				{return m_data.end();}

			uint32_t offsetBeginGet() const noexcept
				{return m_offset_begin;}

			float sampleRateGet() const noexcept
				{return m_fs;}

			Waveform& sampleRateSet(float fs) noexcept
				{
				m_flags|=(std::abs(fs-m_fs)>1e-2? DIRTY : 0);
				m_fs=fs;
				return *this;
				}

			Waveform& offsetBeginSet(int32_t value_new) noexcept
				{
				auto temp=std::min(value_new,static_cast<int32_t>(m_data.length()));
				m_flags|=(temp!=m_offset_begin? DIRTY : 0);
				m_offset_begin=temp;
				return *this;
				}

			uint32_t offsetEndGet() const noexcept
				{return m_offset_end;}

			Waveform& offsetEndSet(int32_t value_new) noexcept
				{
				auto temp=std::min(value_new,static_cast<int32_t>(m_data.length()));
				m_flags|=(temp!=m_offset_end? DIRTY : 0);
				m_offset_end=temp;
				return *this;
				}

			Waveform& offsetsReset() noexcept
				{
				m_offset_end=m_data.length();
				m_offset_begin=0;
				m_flags|=DIRTY;
				return *this;
				}

			Waveform& reverse() noexcept
				{
				std::swap(m_offset_end,m_offset_begin);
				m_flags|=DIRTY;
				return *this;
				}



			uint32_t length() const noexcept
				{
				return reversedIs()?
					m_offset_begin-m_offset_end:m_offset_end-m_offset_begin;
				}

			uint32_t lengthFull() const noexcept
				{return m_data.length();}

			bool reversedIs() const noexcept
				{return m_offset_begin > m_offset_end;}



			uint32_t channelGet() const noexcept
				{return m_channel;}

			Waveform& channelSet(int32_t x) noexcept
				{
				m_flags|=(x!=m_channel? DIRTY : 0);
				m_channel=x;
				return *this;
				}



			float gainGet() const noexcept
				{return m_gain;}

			Waveform& gainSet(float gain) noexcept
				{
				m_flags|=(std::abs(gain - m_gain)>1e-4? DIRTY : 0);
				m_gain=gain;
				return *this;
				}

			float gainRandomGet() const noexcept
				{return m_gain_random;}

			Waveform& gainRandomSet(float value) noexcept
				{
				m_flags|=(std::abs(value - m_gain_random)>1e-4? DIRTY : 0);;
				m_gain_random=value;
				return *this;
				}



			static constexpr uint32_t LOOP=0x1;
			static constexpr uint32_t SUSTAIN=0x2;
			static constexpr uint32_t READONLY=0x4;
			static constexpr uint32_t GAIN_ONLOOP_SET=0x8;
			static constexpr uint32_t RECORDED=0x10000000;
			static constexpr uint32_t PLAYBACK_RUNNING=0x20000000;
			static constexpr uint32_t RECORD_RUNNING=0x40000000;

			uint32_t flagsGet() const noexcept
				{return m_flags;}

			uint32_t flagGet(uint32_t flag_index) const noexcept
				{return m_flags&(1<<flag_index);}

			Waveform& flagsSet(uint32_t flags) noexcept
				{
				auto flags_new=m_flags|flags;
				auto flags_old=m_flags&~MASK_CHANGE_IGNORE;

				m_flags=flags_new;
				m_flags|=( (flags_old== (flags_new&~MASK_CHANGE_IGNORE) )? 0 : DIRTY);
				return *this;
				}

			Waveform& flagSet(uint32_t flag_index) noexcept
				{return flagsSet(1<<flag_index);}

			Waveform& flagUnset(uint32_t flag_index) noexcept
				{return flagsUnset(1<<flag_index);}

			Waveform& flagsUnset(uint32_t flags) noexcept
				{
				auto flags_new=m_flags&~flags;
				auto flags_old=m_flags&~MASK_CHANGE_IGNORE;

				m_flags=flags_new;
				m_flags|=( (flags_old==(flags_new&~MASK_CHANGE_IGNORE) )? 0 : DIRTY);
				return *this;
				}



			Waveform& append(float x)
				{
				m_data.append(x);
				m_flags|=DIRTY;
				return *this;
				}

			Waveform& append(const float* x,uint32_t n)
				{
				m_data.append(x,n);
				m_flags|=DIRTY;
				return *this;
				}

			Waveform& capacitySet(uint32_t capacity_new)
				{
				m_data.capacitySet(capacity_new);
				return *this;
				}

			void clear()
				{
				m_data.clear();
				offsetsReset();
				m_flags|=DIRTY;
				}

			static Waveform nullGet()
				{
				float vals[2]={1.0e-7f,1.0e-7f};
				return Waveform(vals,2,1000.0f);
				}

			Waveform& valuesInit() noexcept
				{
				m_gain=0.0f;
				m_gain_random=0.0f;
				m_channel=0;
				m_flags=0.0f;
				m_offset_begin=0;
				m_offset_end=m_data.length();
				m_flags|=DIRTY;
				return *this;
				}

			bool dirtyIs() const noexcept
				{
				return m_flags&DIRTY;
				}

			void dirtyClear()
				{
				m_flags&=~DIRTY;
				}


		private:
			static constexpr uint32_t DIRTY=0x80000000;
			static constexpr uint32_t MASK_CHANGE_IGNORE=DIRTY|RECORD_RUNNING|PLAYBACK_RUNNING;

			ArrayDynamicShort<float> m_data;
			int32_t m_offset_begin;
			int32_t m_offset_end;
			int32_t m_channel;
			float m_gain;
			float m_gain_random;
			float m_fs;
			uint32_t m_flags;
		};
	}
#endif
