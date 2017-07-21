//@	{
//@	 "targets":[{"name":"waveformproxy.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveformproxy.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORMPROXY_HPP
#define ANJA_WAVEFORMPROXY_HPP

#include "waveform.hpp"
#include "waveformdata.hpp"

namespace Anja
	{
	class SessionFileRecord;

	class WaveformProxy
		{
		public:
			explicit WaveformProxy(Waveform& wf,WaveformData& wd
				,const String& dir_current,const double& fs,int index) noexcept:
				r_waveform(&wf),r_waveform_data(&wd),r_dir_current(&dir_current),r_fs(&fs),m_index(index)
				{}

			template<class ProgressCallback>
			WaveformProxy& load(const SessionFileRecord& rec,ProgressCallback& cb)
				{
				return load(rec,[](void* cb_obj,WaveformProxy& waveform,float status)
					{
					reinterpret_cast<ProgressCallback*>(cb_obj)->progressLoad(waveform,status);
					},&cb);
				}

			const WaveformProxy& store(SessionFileRecord& rec) const;

			String filename() const;

			const String& directory() const noexcept
				{return *r_dir_current;}

			WaveformProxy& filename(const String& filename);

			const String& description() const noexcept
				{return r_waveform_data->description();}

			WaveformProxy& description(const char* description)
				{
				r_waveform_data->description(description);
				return *this;
				}

			WaveformProxy& description(String&& description)
				{
				r_waveform_data->description(std::move(description));
				return *this;
				}

			const String& keyLabel() const noexcept
				{return r_waveform_data->keyLabel();;}

			const ColorRGBA& keyColor() const noexcept
				{return r_waveform_data->keyColor();;}

			WaveformProxy& keyColorSet(const ColorRGBA& color_new)
				{
				r_waveform_data->keyColor(color_new);
				return *this;
				}

			template<class ProgressCallback>
			WaveformProxy& waveformLoad(const char* filename,ProgressCallback& cb)
				{
				return waveformLoad(filename,[](void* cb_obj,WaveformProxy& waveform,float status)
					{
					reinterpret_cast<ProgressCallback*>(cb_obj)->progressLoad(waveform,status);
					},&cb);
				}

			template<class ProgressCallback>
			WaveformProxy& resample(double fs,ProgressCallback& cb)
				{
				struct
					{
					ProgressCallback& cb;
					WaveformProxy& self;
					void progressResample(Waveform& w,float status)
						{cb.progressResample(self,status);}
					void progressLoad(WaveformProxy& w,float status)
						{cb.progressResample(w,status);}
					} cbobj{cb,*this};

			/*	Reload. FIXME: Reloading waveform must also reset the cursors
				if(r_waveform->resampled())
					{
					fprintf(stderr,"Dirty %d    ",r_waveform->dirty());
					waveformLoad(r_waveform_data->filenameGet().begin(),cbobj);
					r_waveform->dirtyClear();
					fprintf(stderr,"Dirty %d\n",r_waveform->dirty());

					}*/
				r_waveform->resample(fs,cbobj);
				return *this;
				}


			const WaveformProxy& waveformSave(const char* filename) const;

			bool loadPossible(const char* filename) const;

			bool waveformLoaded(const char* filename) const;

			template<Waveform::Cursor c>
			const float* pointer() const noexcept
				{return r_waveform->pointer<c>();}

			template<Waveform::Cursor c>
			float* pointer() noexcept
				{return r_waveform->pointer<c>();}

			const float* beginFull() const noexcept
				{return r_waveform->beginFull();}

			const float* endFull() const noexcept
				{return r_waveform->endFull();}

			float sampleRate() const noexcept
				{return r_waveform->sampleRate();}

			WaveformProxy& sampleRate(float fs) noexcept
				{
				r_waveform->sampleRate(fs);
				return *this;
				}

			template<Waveform::Cursor c>
			int32_t offset() const noexcept
				{return r_waveform->offset<c>();}

			template<Waveform::Cursor c>
			WaveformProxy& offset(int32_t val_new) noexcept
				{
				r_waveform->offset<c>(val_new);
				return *this;
				}

			template<Waveform::Cursor c>
			WaveformProxy& offset(double val_new) noexcept
				{
				r_waveform->offset<c>(val_new);
				return *this;
				}

			WaveformProxy& offsetsReset() noexcept
				{
				r_waveform->offsetsReset();
				return *this;
				}

			WaveformProxy& reverse() noexcept
				{
				r_waveform->reverse();
				return *this;
				}

			uint32_t length() const noexcept
				{return r_waveform->length();}

			uint32_t lengthFull() const noexcept
				{return r_waveform->lengthFull();}

			int direction() const noexcept
				{return r_waveform->direction();}

			uint32_t channel() const noexcept
				{return r_waveform->channel();}

			WaveformProxy& channel(int32_t x) noexcept
				{
				r_waveform->channel(x);
				return *this;
				}



			float gain() const noexcept
				{return r_waveform->gain();}

			WaveformProxy& gainSet(float gain) noexcept
				{
				r_waveform->gain(gain);
				return *this;
				}

			float gainRandom() const noexcept
				{return r_waveform->gainRandom();}

			void gainRandomSet(float value) noexcept
				{r_waveform->gainRandom(value);}

			uint32_t flags() const noexcept
				{return r_waveform->flags();}

			uint32_t flag(uint32_t flag_index) const noexcept
				{return r_waveform->flag(flag_index);}

			WaveformProxy& flagsSet(uint32_t flags) noexcept
				{
				r_waveform->flagsSet(flags);
				return *this;
				}

			WaveformProxy& flagSet(uint32_t flag_index) noexcept
				{
				r_waveform->flagSet(flag_index);
				return *this;
				}

			WaveformProxy& flagUnset(uint32_t flag_index) noexcept
				{
				r_waveform->flagUnset(flag_index);
				return *this;
				}

			WaveformProxy& flagsUnset(uint32_t flags) noexcept
				{
				r_waveform->flagsUnset(flags);
				return *this;
				}

			WaveformProxy& append(const float* x,uint32_t n)
				{
				r_waveform->append(x,n);
				return *this;
				}

			WaveformProxy& capacitySet(uint32_t capacity_new)
				{
				r_waveform->capacity(capacity_new);
				return *this;
				}

			bool recorded() const noexcept
				{return r_waveform->recorded();}



			bool dirty() const noexcept
				{return r_waveform_data->dirty() || r_waveform->dirty();}

			WaveformProxy& dirtyClear()
				{
				r_waveform_data->dirtyClear();
				r_waveform->dirtyClear();
				return *this;
				}

			static const char* const* flagNames() noexcept
				{return Waveform::flagNames();}

			int index() const noexcept
				{return m_index;}

		private:
			Waveform* r_waveform;
			WaveformData* r_waveform_data;
			const String* r_dir_current;
			const double* r_fs;
			int m_index;

			typedef void (*progress_callback)(void* cb_obj,WaveformProxy& waveform,float status);
			WaveformProxy& load(const SessionFileRecord& rec,progress_callback cb,void* cb_obj);
			WaveformProxy& waveformLoad(const char* filename,progress_callback cb,void* cb_obj);
		};
	}

#endif // ANJA_WAVEFORMVIEW_HPP
