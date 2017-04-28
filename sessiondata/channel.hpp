//@	{
//@	"targets":[{"name":"channel.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channel.o","rel":"implementation"}]
//@	}


#ifndef ANJA_CHANNEL_HPP
#define ANJA_CHANNEL_HPP

#include <algorithm>
#include <cstdint>

namespace Anja
	{
	class SessionFileRecord;

	class Channel
		{
		public:
			Channel() noexcept
				{valuesInit();}

			explicit Channel(const SessionFileRecord& record);

			void dataGet(SessionFileRecord& record) const;

			void valuesInit() noexcept
				{
				m_gain=0.0f;
				m_fade_time=1.0f;
				dirtyClear();
				}

			float gainGet() const noexcept
				{return m_gain;}

			void gainSet(float gain) noexcept
				{
				m_state_flags|=(std::abs(gain-m_gain)>1e-4? DIRTY : 0);
				m_gain=gain;
				}

			float fadeTimeGet() const noexcept
				{
				return m_fade_time;
				}

			void fadeTimeSet(float time) noexcept
				{
				auto temp=std::max(time,1e-3f);
				m_state_flags|=(std::abs(temp - m_fade_time)>1e-6? DIRTY : 0);
				m_fade_time=temp;
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
	 }
#endif
