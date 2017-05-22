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

			void clear();

			void dataSet(const SessionFileRecord& rec)
				{*this=WaveformData(rec);}


			const String& filenameGet() const noexcept
				{return m_filename;}

			void filenameSet(const char* filename)
				{
				m_filename=String(filename);
				m_stateflags|=DIRTY;
				}

			void filenameSet(String&& filename) noexcept
				{
				m_filename=std::move(filename);
				m_stateflags|=DIRTY;
				}

			const String& descriptionGet() const noexcept
				{return m_description;}

			void descriptionSet(String&& description)
				{
				m_description=std::move(description);
				m_stateflags|=DIRTY;
				key_label_update();
				}

			void descriptionSet(const char* desc)
				{
				m_description=String(desc);
				m_stateflags|=DIRTY;
				key_label_update();
				}

			const String& keyLabelGet() const noexcept
				{return m_key_label;}



			const ColorRGBA& keyColorGet() const noexcept
				{return m_color;}

			void keyColorSet(const ColorRGBA& color_new);

			void dataGet(SessionFileRecord& record) const;

			bool dirtyIs() const noexcept
				{return m_stateflags&DIRTY;}

			void dirtyClear()
				{m_stateflags&=~DIRTY;}

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
