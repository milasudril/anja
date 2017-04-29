//@	{
//@	 "targets":[{"name":"waveformview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMVIEW_HPP
#define ANJA_WAVEFORMVIEW_HPP

#include "waveform.hpp"
#include "waveformdata.hpp"

namespace Anja
	{
	class SessionFileRecord;

	class WaveformView
		{
		public:
			explicit WaveformView(Waveform& wf,WaveformData& wd) noexcept:
				r_waveform(wf),r_waveform_data(wd)
				{}

			void load(const SessionFileRecord& rec);
			void store(SessionFileRecord& rec);


			const ArrayDynamicShort<char>& filenameGet() const
				{return r_waveform_data.filenameGet();}

			void filenameSet(const ArrayDynamicShort<char>& filename);

			const ArrayDynamicShort<char>& descriptionGet() const noexcept
				{return r_waveform_data.descriptionGet();}

			void descriptionSet(const ArrayDynamicShort<char>& description)
				{r_waveform_data.descriptionSet(description);}

			const ArrayDynamicShort<char>& keyLabelGet() const noexcept
				{return r_waveform_data.keyLabelGet();;}

			const ColorRGBA& keyColorGet() const noexcept
				{return r_waveform_data.keyColorGet();;}

			void keyColorSet(const ColorRGBA& color_new)
				{r_waveform_data.keyColorSet(color_new);}

			bool dirtyIs() const noexcept
				{return r_waveform_data.dirtyIs() || r_waveform.dirtyIs();}

			void dirtyClear()
				{
				r_waveform_data.dirtyClear();
				r_waveform.dirtyClear();
				}
			

		private:
			Waveform& r_waveform;
			WaveformData& r_waveform_data;
		};
	}

#endif // ANJA_WAVEFORMVIEW_HPP
