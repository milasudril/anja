//@	{
//@	 "targets":[{"name":"waveformview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMVIEW_HPP
#define ANJA_WAVEFORMVIEW_HPP

#include "waveform.hpp"
#include "waveformdata.hpp"
#include "../common/keyboardlayout.hpp"

namespace Anja
	{
	class SessionFileRecord;

	class WaveformView
		{
		public:
			explicit WaveformView(Waveform& wf,WaveformData& wd
				,KeyboardLayout::KeyDescriptor* key) noexcept:
				r_waveform(wf),r_waveform_data(wd),r_key(key)
				{}

			void load(const SessionFileRecord& rec,const String& dir_current);

			void store(SessionFileRecord& rec,const String& dir_current);

			const String& filenameGet() const
				{return r_waveform_data.filenameGet();}

			void filenameSet(const String& filename);

			const String& descriptionGet() const noexcept
				{return r_waveform_data.descriptionGet();}

			void descriptionSet(const String& description)
				{
				r_waveform_data.descriptionSet(description);
				if(r_key!=nullptr)
					{r_key->labelSet(r_waveform_data.keyLabelGet().begin());}
				}

			const String& keyLabelGet() const noexcept
				{return r_waveform_data.keyLabelGet();;}

			const ColorRGBA& keyColorGet() const noexcept
				{return r_waveform_data.keyColorGet();;}

			void keyColorSet(const ColorRGBA& color_new)
				{
				r_waveform_data.keyColorSet(color_new);
				if(r_key!=nullptr)
					{r_key->colorBackgroundSet(r_waveform_data.keyColorGet());}
				}

			Waveform& waveform() noexcept
				{return r_waveform;}

			const Waveform& waveform() const noexcept
				{return r_waveform;}

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
			KeyboardLayout::KeyDescriptor* r_key;
		};
	}

#endif // ANJA_WAVEFORMVIEW_HPP
