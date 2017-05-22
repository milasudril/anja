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
			explicit WaveformView(Waveform& wf,WaveformData& wd
				,const String& dir_current,int index) noexcept:
				r_waveform(&wf),r_waveform_data(&wd),r_dir_current(&dir_current),m_index(index)
				{}

			void load(const SessionFileRecord& rec);

			void store(SessionFileRecord& rec);

			String filenameGet() const;

			const String& directoryGet() const noexcept
				{return *r_dir_current;}

			void filenameSet(const String& filename);

			const String& descriptionGet() const noexcept
				{return r_waveform_data->descriptionGet();}

			void descriptionSet(const char* description)
				{r_waveform_data->descriptionSet(description);}

			void descriptionSet(String&& description)
				{r_waveform_data->descriptionSet(std::move(description));}

			const String& keyLabelGet() const noexcept
				{return r_waveform_data->keyLabelGet();;}

			const ColorRGBA& keyColorGet() const noexcept
				{return r_waveform_data->keyColorGet();;}

			void keyColorSet(const ColorRGBA& color_new)
				{r_waveform_data->keyColorSet(color_new);}

			void fileLoad(const char* filename);

			void fileSave(const char* filename) const;

			bool loadPossible(const char* filename) const;

			bool fileLoaded(const char* filename) const;

			const float* begin() const noexcept
				{return r_waveform->begin();}

			const float* beginFull() const noexcept
				{return r_waveform->beginFull();}

			const float* end() const noexcept
				{return r_waveform->end();}

			const float* endFull() const noexcept
				{return r_waveform->endFull();}

			float sampleRateGet() const noexcept
				{return r_waveform->sampleRateGet();}

			void sampleRateSet(float fs) noexcept
				{r_waveform->sampleRateSet(fs);}

			int32_t offsetBeginGet() const noexcept
				{return r_waveform->offsetBeginGet();}

			void offsetBeginSet(int32_t value_new) noexcept
				{r_waveform->offsetBeginSet(value_new);}

			void offsetBeginSet(double value_new) noexcept
				{
				r_waveform->offsetBeginSet(static_cast<int32_t>(value_new*r_waveform->sampleRateGet()));
				}

			int32_t offsetEndGet() const noexcept
				{return r_waveform->offsetEndGet();}

			void offsetEndSet(int32_t value_new) noexcept
				{r_waveform->offsetEndSet(value_new);}

			void offsetEndSet(double value_new) noexcept
				{
				r_waveform->offsetEndSet(static_cast<int32_t>(value_new*r_waveform->sampleRateGet()));
				}

			void offsetsReset() noexcept
				{r_waveform->offsetsReset();}

			void reverse() noexcept
				{r_waveform->reverse();}

			uint32_t length() const noexcept
				{return r_waveform->length();}

			uint32_t lengthFull() const noexcept
				{return r_waveform->lengthFull();}

			bool reversedIs() const noexcept
				{return r_waveform->reversedIs();}

			uint32_t channelGet() const noexcept
				{return r_waveform->channelGet();}

			void channelSet(int32_t x) noexcept
				{r_waveform->channelSet(x);}



			float gainGet() const noexcept
				{return r_waveform->gainGet();}

			void gainSet(float gain) noexcept
				{return r_waveform->gainSet(gain);}

			float gainRandomGet() const noexcept
				{return r_waveform->gainRandomGet();}

			void gainRandomSet(float value) noexcept
				{r_waveform->gainRandomSet(value);}

			uint32_t flagsGet() const noexcept
				{return r_waveform->flagsGet();}

			uint32_t flagGet(uint32_t flag_index) const noexcept
				{return r_waveform->flagGet(flag_index);}

			void flagsSet(uint32_t flags) noexcept
				{r_waveform->flagsSet(flags);}

			void flagSet(uint32_t flag_index) noexcept
				{r_waveform->flagSet(flag_index);}

			void flagUnset(uint32_t flag_index) noexcept
				{r_waveform->flagUnset(flag_index);}

			void flagsUnset(uint32_t flags) noexcept
				{r_waveform->flagsUnset(flags);}

			void append(const float* x,uint32_t n)
				{r_waveform->append(x,n);}

			void capacitySet(uint32_t capacity_new)
				{r_waveform->capacitySet(capacity_new);}



			bool dirtyIs() const noexcept
				{return r_waveform_data->dirtyIs() || r_waveform->dirtyIs();}

			void dirtyClear()
				{
				r_waveform_data->dirtyClear();
				r_waveform->dirtyClear();
				}

			static const char* const* flagNames() noexcept
				{return Waveform::flagNames();}

			int index() const noexcept
				{return m_index;}

		private:
			Waveform* r_waveform;
			WaveformData* r_waveform_data;
			const String* r_dir_current;
			int m_index;
		};
	}

#endif // ANJA_WAVEFORMVIEW_HPP
