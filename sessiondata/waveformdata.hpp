//@	{
//@	 "targets":[{"name":"waveformdata.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformdata.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMDATA_HPP
#define ANJA_WAVEFORMDATA_HPP

#include "waveform.hpp"
#include "../common/color.hpp"

namespace Anja
	{
	class SessionFileRecord;
	class ColorString;

	class WaveformData
		{
		public:
			WaveformData();

			WaveformData(const SessionFileRecord& record
				,const ArrayDynamicShort<char>& load_path);


			const ArrayDynamicShort<char>& filenameGet() const
				{return m_filename;}

			void filenameSet(const ArrayDynamicShort<char>& filename
				,const ArrayDynamicShort<char>& load_path);

			void filenameSet(const char* filename);

			void sampleRateSet(float fs);



			const ArrayDynamicShort<char>& descriptionGet() const noexcept
				{return m_description;}

			void descriptionSet(const ArrayDynamicShort<char>& description);

			void descriptionSet(const char* description);

			const ArrayDynamicShort<char>& keyLabelGet() const noexcept
				{return m_key_label;}



			const ColorRGBA& keyColorGet() const noexcept
				{return m_color;}

			void keyColorSet(const ColorRGBA& color_new);

			void dataGet(SessionFileRecord& record
				,const ArrayDynamicShort<char>& load_path) const;

			bool dirtyIs() const noexcept
				{return m_stateflags&DIRTY;}

			void dirtyClear()
				{m_stateflags&=~DIRTY;}

		private:
			ArrayDynamicShort<char> m_filename;
			ArrayDynamicShort<char> m_description;
			ArrayDynamicShort<char> m_key_label;
			ColorRGBA m_color;

			static constexpr unsigned int DIRTY=0x1;

			unsigned int m_stateflags;
		};
	}
#endif
