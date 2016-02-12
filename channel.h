#ifdef __WAND__
target[name[channel.h] type[include]]
#endif

#ifndef CHANNEL_H
#define CHANNEL_H

#include <algorithm>
#include <cstdint>

class Channel
	{
	public:
		Channel& valuesInit() noexcept
			{
			m_gain=0.0f;
			m_fade_time=1.0f;
			dirtyClear();
			return *this;
			}

		float gainGet() const noexcept
			{return m_gain;}

		Channel& gainSet(float gain) noexcept
			{
			m_state_flags|=(std::abs(gain-m_gain)>1e-4? DIRTY : 0);
			m_gain=gain;
			return *this;
			}

		float fadeTimeGet() const noexcept
			{
			return m_fade_time;
			}

		Channel& fadeTimeSet(float time) noexcept
			{
			auto temp=std::max(time,1e-3f);
			m_state_flags|=(std::abs(temp - m_fade_time)>1e-6? DIRTY : 0);
			m_fade_time=temp;
			return *this;
			}

		bool dirtyIs() const noexcept
			{
			return m_state_flags&DIRTY;
			}

		void dirtyClear() noexcept
			{m_state_flags&=~DIRTY;}

	private:
		float m_gain;
		float m_fade_time;

		static constexpr uint32_t DIRTY=0x1;
		uint32_t m_state_flags;
	};

#endif
