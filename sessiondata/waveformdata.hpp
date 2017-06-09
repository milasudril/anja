//@	{
//@	 "targets":[{"name":"waveformdata.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformdata.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMDATA_HPP
#define ANJA_WAVEFORMDATA_HPP

#include "waveform.hpp"
#include "../common/string.hpp"
#include "../common/color.hpp"

namespace Anja
	{
	class SessionFileRecord;
	class ColorString;

	class WaveformData
		{
		public:
			WaveformData()
				{clear();}

			explicit WaveformData(const SessionFileRecord& record);

			WaveformData& clear();

			WaveformData& load(const SessionFileRecord& rec)
				{
				*this=WaveformData(rec);
				return *this;
				}

			const String& filenameGet() const noexcept
				{return m_filename;}

			WaveformData& filename(const char* filename)
				{
				if(m_filename!=filename)
					{
					m_filename=String(filename);
					m_stateflags|=DIRTY;
					}
				return *this;
				}

			WaveformData& filename(String&& filename) noexcept
				{
				if(m_filename!=filename)
					{
					m_filename=std::move(filename);
					m_stateflags|=DIRTY;
					}
				return *this;
				}

			const String& description() const noexcept
				{return m_description;}

			WaveformData& description(String&& description) noexcept
				{
				if(m_description!=description)
					{
					m_description=std::move(description);
					m_stateflags|=DIRTY;
					key_label_update();
					}
				return *this;
				}

			WaveformData& description(const char* desc)
				{
				if(m_description!=desc)
					{
					m_description=String(desc);
					m_stateflags|=DIRTY;
					key_label_update();
					}
				return *this;
				}

			const String& keyLabel() const noexcept
				{return m_key_label;}



			const ColorRGBA& keyColor() const noexcept
				{return m_color;}

			WaveformData& keyColor(const ColorRGBA& color_new);

			const WaveformData& store(SessionFileRecord& record) const;

			bool dirty() const noexcept
				{return m_stateflags&DIRTY;}

			WaveformData& dirtyClear() noexcept
				{
				m_stateflags&=~DIRTY;
				return *this;
				}

		private:
			String m_filename;
			String m_description;
			String m_key_label;
			ColorRGBA m_color;

			static constexpr unsigned int DIRTY=0x1;

			unsigned int m_stateflags;

			void key_label_update();
		};
	}
#endif
