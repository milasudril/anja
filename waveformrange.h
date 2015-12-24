#ifdef __WAND__
target[name[waveformrange.h] type[include]]
#endif

#ifndef WAVEFORMRANGE_H
#define WAVEFORMRANGE_H

#include <algorithm>

class WaveformRange
	{
	public:
		constexpr WaveformRange(const float* buffer,uint32_t buffer_size,float fs):
			r_buffer(buffer),m_offset_begin(0),m_offset_end(buffer_size)
			,m_offset_end_max(buffer_size),m_fs(fs)
			{}

		constexpr const float* begin() const
			{return r_buffer + m_offset_begin;}

		constexpr const float* beginFull() const
			{return r_buffer;}

		constexpr const float* end() const
			{return r_buffer + m_offset_end;}

		constexpr const float* endFull() const
			{return r_buffer + m_offset_end_max;}

		constexpr uint32_t offsetBeginGet() const
			{return m_offset_begin;}

		constexpr float sampleRateGet() const
			{return m_fs;}

		WaveformRange& offsetBeginSet(uint32_t value_new)
			{
			m_offset_begin=std::min(value_new,m_offset_end_max);
			return *this;
			}

		constexpr uint32_t offsetEndGet() const
			{return m_offset_end;}

		WaveformRange& offsetEndSet(uint32_t value_new)
			{
			m_offset_end=std::min(value_new,m_offset_end_max);
			return *this;
			}

		constexpr uint32_t length() const
			{return m_offset_end-m_offset_begin;}

		constexpr uint32_t lengthFull() const
			{return m_offset_end_max;}

		constexpr bool reversedIs() const
			{return m_offset_begin > m_offset_end;}

	private:
		const float* r_buffer;
		uint32_t m_offset_begin;
		uint32_t m_offset_end;
		uint32_t m_offset_end_max;
		float m_fs;
	};

#endif
