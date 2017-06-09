//@	{
//@	 "targets":[{"name":"channeldata.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channeldata.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CHANNELDATA_HPP
#define ANJA_CHANNELDATA_HPP

#include "../common/color.hpp"
#include "../common/string.hpp"

namespace Anja
	{
	class SessionFileRecord;
	class ChannelData
		{
		public:
			ChannelData()
				{clear();}

			ChannelData& clear();

			explicit ChannelData(const SessionFileRecord& record);

			const String& label() const noexcept
				{return m_label;}

			ChannelData& label(const char* label)
				{
				if(m_label!=label)
					{
					m_label=String(label);
					m_state_flags|=DIRTY;
					}
				return *this;
				}

			ChannelData& label(String&& label) noexcept
				{
				if(m_label!=label)
					{
					m_label=std::move(label);
					m_state_flags|=DIRTY;
					}
				return *this;
				}

			const ColorRGBA& color() const noexcept
				{return m_color;}

			ChannelData& color(const ColorRGBA& color) noexcept;

			const ChannelData& store(SessionFileRecord& record) const;

			bool dirty() const noexcept
				{return m_state_flags&DIRTY;}

			void dirtyClear() noexcept
				{m_state_flags&=~DIRTY;}

		private:
			String m_label;
			ColorRGBA m_color;

			static constexpr unsigned int DIRTY=0x1;

			unsigned int m_state_flags;
		};
	}

#endif
