#ifdef __WAND__
target[name[waveform.h] type[include]]
#endif

#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <algorithm>
#include <cstdint>

template< class ArrayDescriptor >
class Waveform
	{
	public:
		constexpr Waveform(const float* buffer,uint32_t buffer_size,float fs):
			m_data(buffer,buffer_size),m_offset_begin(0),m_offset_end(buffer_size)
			,m_gain(1.0f),m_fs(fs),m_flags(0)
			{}

		constexpr const float* begin() const
			{return m_data.begin() + m_offset_begin;}

		constexpr const float* beginFull() const
			{return m_data.begin();}

		constexpr const float* end() const
			{return m_data.begin() + m_offset_end;}

		constexpr const float* endFull() const
			{return m_data.end();}

		constexpr uint32_t offsetBeginGet() const
			{return m_offset_begin;}

		constexpr float sampleRateGet() const
			{return m_fs;}

		void sampleRateSet(float fs)
			{m_fs=fs;}

		Waveform& offsetBeginSet(uint32_t value_new)
			{
			m_offset_begin=std::min(value_new,m_data.length());
			return *this;
			}

		constexpr uint32_t offsetEndGet() const
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


		constexpr uint32_t length() const
			{return m_offset_end-m_offset_begin;}

		constexpr uint32_t lengthFull() const
			{return m_data.length();}

		constexpr bool reversedIs() const
			{return m_offset_begin > m_offset_end;}

		constexpr float gainGet() const
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

		constexpr uint32_t flagsGet() const
			{return m_flags;}

		Waveform& flagsSet(uint32_t flags)
			{
			m_flags=flags;
			return *this;
			}

	protected:
		ArrayDescriptor m_data;

	private:
		uint32_t m_offset_begin;
		uint32_t m_offset_end;
		float m_gain;
		float m_fs;
		uint32_t m_flags;
	};

#endif
