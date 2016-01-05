#ifdef __WAND__
target[name[waveform.h] type[include]]
#endif

#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "framework/arraydynamicshort.h"

#include <algorithm>
#include <cstdint>

class Waveform
	{
	public:
		Waveform():m_offset_begin(0),m_offset_end(0)
			,m_gain(1.0f),m_fs(0),m_flags(0)
			{}

		Waveform(const float* buffer,uint32_t buffer_size,float fs):
			m_data(buffer,buffer_size),m_offset_begin(0),m_offset_end(buffer_size)
			,m_gain(1.0f),m_fs(fs),m_flags(0)
			{}

		const float* begin() const
			{return m_data.begin() + m_offset_begin;}

		const float* beginFull() const
			{return m_data.begin();}

		const float* end() const
			{return m_data.begin() + m_offset_end;}

		const float* endFull() const
			{return m_data.end();}

		uint32_t offsetBeginGet() const
			{return m_offset_begin;}

		float sampleRateGet() const
			{return m_fs;}

		void sampleRateSet(float fs)
			{m_fs=fs;}

		Waveform& offsetBeginSet(uint32_t value_new)
			{
			m_offset_begin=std::min(value_new,m_data.length());
			return *this;
			}

		uint32_t offsetEndGet() const
			{return m_offset_end;}

		Waveform& offsetEndSet(uint32_t value_new)
			{
			m_offset_end=std::min(value_new,m_data.length());
			return *this;
			}

		Waveform& offsetsReset()
			{
			m_offset_end=m_data.length();
			m_offset_begin=0;
			return *this;
			}

		Waveform& reverse()
			{
			std::swap(m_offset_end,m_offset_begin);
			return *this;
			}



		uint32_t length() const
			{
			return reversedIs()?
				m_offset_begin-m_offset_end:m_offset_end-m_offset_begin;
			}

		uint32_t lengthFull() const
			{return m_data.length();}

		bool reversedIs() const
			{return m_offset_begin > m_offset_end;}

		float gainGet() const
			{return m_gain;}

		Waveform& gainSet(float gain)
			{
			m_gain=gain;
			return *this;
			}

		static constexpr uint32_t READONLY=1;
		static constexpr uint32_t GAIN_RANDOMIZE=2;
		static constexpr uint32_t PITCH_RANDOMIZE=4;
		static constexpr uint32_t LOOP=8;
		static constexpr uint32_t SUSTAIN=16;

		uint32_t flagsGet() const
			{return m_flags;}

		Waveform& flagsSet(uint32_t flags)
			{
			m_flags=flags;
			return *this;
			}


		Waveform& append(float x)
			{
			m_data.append(x);
			return *this;
			}

		Waveform& append(const float* x,uint32_t n)
			{
			m_data.append(x,n);
			return *this;
			}

		Waveform& capacitySet(uint32_t capacity_new)
			{
			m_data.capacitySet(capacity_new);
			return *this;
			}

		void clear()
			{m_data.clear();}

		static Waveform nullGet()
			{
			float vals[2]={1.0e-7f,1.0e-7f};
			return Waveform(vals,2,1000.0f);
			}

	private:
		ArrayDynamicShort<float> m_data;
		uint32_t m_offset_begin;
		uint32_t m_offset_end;
		float m_gain;
		float m_fs;
		uint32_t m_flags;
	};

#endif
