//@	{
//@	 "targets":[{"name":"waveform.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"waveform.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WAVEFORM_HPP
#define ANJA_WAVEFORM_HPP

#include "../common/arraydynamicshort.hpp"
#include "../common/mutex.hpp"

#include <algorithm>
#include <cstdint>

namespace Anja
	{
	class SessionFileRecord;

	class Waveform
		{
		public:
			Waveform() noexcept:m_channel(0)
				,m_gain(0.0f),m_gain_random(0.0f),m_fs(0),m_length_ratio(1.0),m_flags(0)
				{}

			explicit Waveform(const float* buffer,uint32_t buffer_size,float fs) noexcept:
				m_gain(0.0f),m_gain_random(0.0f),m_fs(fs),m_length_ratio(1.0),m_flags(READONLY)
				{
				m_data.append(buffer,buffer_size);
				offsetsReset();
				}

			const Waveform& waveformSave(const char* filename) const;

			static bool loadPossible(const char* filename);

			const Waveform& store(SessionFileRecord& record) const;

			Waveform& load(const SessionFileRecord& rec)
				{
				Mutex::LockGuardNonblocking lock(m_mtx);
				*this=Waveform(rec,nullptr,nullptr,nullptr);
				return *this;
				}

			template<class ProgressCallback>
			Waveform& load(const SessionFileRecord& rec,const char* filename
				,ProgressCallback& cb)
				{
				return load(rec,filename,[](void* cb_obj,Waveform& self,float status)
					{
					reinterpret_cast<ProgressCallback*>(cb_obj)->progressLoad(self,status);
					},&cb);
				}

			template<class ProgressCallback>
			Waveform& waveformLoad(const char* filename,ProgressCallback& cb)
				{
				return waveformLoad(filename,[](void* cb_obj,Waveform& self,float status)
					{
					reinterpret_cast<ProgressCallback*>(cb_obj)->progressLoad(self,status);
					},&cb);
				}

			template<class ProgressCallback>
			Waveform& resample(double fs,ProgressCallback& cb)
				{
				return resample(fs,[](void* cb_obj,Waveform& self,float status)
					{
					reinterpret_cast<ProgressCallback*>(cb_obj)->progressResample(self,status);
					},&cb);
				}


			enum class Cursor:int32_t{BEGIN,BEGIN_LOOP,END_LOOP,END};

			template<Cursor c>
			int32_t offset() const noexcept
				{
				static_assert(static_cast<int>(c)>=0 && static_cast<int>(c)<4,"");
				return m_offsets[ static_cast<int32_t>(c) ]*m_length_ratio;
				}

			template<Cursor c>
			Waveform& offset(int32_t value_new) noexcept
				{
				static_assert(static_cast<int32_t>(c)>=0 && static_cast<int32_t>(c)<4,"");
				value_new= value_new/m_length_ratio + 0.5;
				auto temp=std::max(std::min(value_new,static_cast<int32_t>(m_data.length())),0);
				m_flags|=(temp!=m_offsets[ static_cast<int32_t>(c) ]? DIRTY : 0);
				m_offsets[ static_cast<int32_t>(c) ]=temp;

				m_offsets[ 1 ]=clamp(m_offsets[0],m_offsets[3],m_offsets[1]);
				m_offsets[ 2 ]=clamp(m_offsets[0],m_offsets[3],m_offsets[2]);
				switch(c)
					{
					case Cursor::BEGIN_LOOP:
						m_offsets[ 2 ]=clamp(m_offsets[1],m_offsets[3],m_offsets[2]);
						break;
					case Cursor::END_LOOP:
						m_offsets[ 1 ]=clamp(m_offsets[0],m_offsets[2],m_offsets[1]);
						break;
					default:
						break;
					}
				return *this;
				}

			template<Waveform::Cursor c>
			Waveform& offset(double val_new) noexcept
				{return offset<c>(static_cast<int32_t>(val_new*m_fs));}

			template<Cursor c>
			const float* pointer() const noexcept
				{return m_data.begin() + offset<c>();}

			template<Cursor c>
			float* pointer() noexcept
				{return m_data.begin() + offset<c>();}


			const float* beginFull() const noexcept
				{return m_data.begin();}

			const float* endFull() const noexcept
				{return m_data.end();}

			double sampleRate() const noexcept
				{return m_fs;}

			Waveform& sampleRate(float fs) noexcept
				{
				m_flags|=(std::abs(fs-m_fs)>1e-2? DIRTY : 0);
				m_fs=fs;
				return *this;
				}

			Waveform& offsetsReset() noexcept
				{
				m_offsets[0]=0;
				m_offsets[1]=0;
				m_offsets[2]=m_data.length()/m_length_ratio;
				m_offsets[3]=m_data.length()/m_length_ratio;
				m_flags|=DIRTY;
				return *this;
				}

			Waveform& reverse() noexcept
				{
				std::swap(m_offsets[0],m_offsets[3]);
				std::swap(m_offsets[1],m_offsets[2]);
				m_flags|=DIRTY;
				return *this;
				}



			uint32_t length() const noexcept
				{return std::abs(offset<Cursor::END>() - offset<Cursor::BEGIN>());}

			uint32_t lengthFull() const noexcept
				{return m_data.length();}

			int direction() const noexcept
				{return offset<Cursor::END>() > offset<Cursor::BEGIN>()?1:-1;}



			int32_t channel() const noexcept
				{return m_channel;}

			Waveform& channel(int32_t x) noexcept
				{
				assert(x>=0 && x<16);
				m_flags|=(x!=m_channel? DIRTY : 0);
				m_channel=x;
				return *this;
				}



			float gain() const noexcept
				{return m_gain;}

			Waveform& gain(float gain) noexcept
				{
				m_flags|=(std::abs(gain - m_gain)>1e-4? DIRTY : 0);
				m_gain=gain;
				return *this;
				}

			float gainRandom() const noexcept
				{return m_gain_random;}

			Waveform& gainRandom(float value) noexcept
				{
				m_flags|=(std::abs(value - m_gain_random)>1e-4? DIRTY : 0);;
				m_gain_random=value;
				return *this;
				}


			static constexpr uint32_t LOOP=0x1;
			static constexpr uint32_t SUSTAIN=0x2;
			static constexpr uint32_t READONLY=0x4;
			static constexpr uint32_t GAIN_ONLOOP_SET=0x8;

			uint32_t flags() const noexcept
				{return m_flags& ( ~DIRTY );}

			uint32_t flag(uint32_t flag_index) const noexcept
				{return m_flags&(1<<flag_index);}

			Waveform& flagsSet(uint32_t f) noexcept
				{
				auto flags_old=(m_flags & ~(RESAMPLED|DIRTY)) ;
				m_flags|=f;
				m_flags|=( flags_old==m_flags )? 0 : DIRTY;
				return *this;
				}

			Waveform& flagSet(uint32_t flag_index) noexcept
				{return flagsSet(1<<flag_index);}

			Waveform& flagsUnset(uint32_t f) noexcept
				{
				auto flags_old=(m_flags & ~(RESAMPLED|DIRTY)) ;
				m_flags&=~f;
				m_flags|=( flags_old==m_flags )? 0 : DIRTY;
				return *this;
				}

			Waveform& flagUnset(uint32_t flag_index) noexcept
				{return flagsUnset(1<<flag_index);}

			static const char* const* flagNames() noexcept;

			Waveform& append(const float* x,uint32_t n)
				{
				m_data.append(x,n);
				m_flags|=DIRTY;
				return *this;
				}

			Waveform& capacity(uint32_t capacity_new)
				{
				m_data.capacity(capacity_new);
				return *this;
				}

			Waveform& reset() noexcept
				{
				Mutex::LockGuardNonblocking lock(m_mtx);
				m_gain=0.0f;
				m_gain_random=0.0f;
				m_channel=0;
				m_flags=0.0f;
				m_fs=1000.0f;
				m_data.clear();
				offsetsReset();
				m_flags=0;
				m_length_ratio=1.0;
				return *this;
				}

			Waveform& clear() noexcept
				{
				m_data.clear();
				return *this;
				}

			static Waveform nullGet()
				{
				float vals[2]={1.0e-7f,1.0e-7f};
				return Waveform(vals,2,1000.0f);
				}

			bool dirty() const noexcept
				{
				return m_flags&DIRTY;
				}

			Waveform& dirtyClear()
				{
				m_flags&=~DIRTY;
				return *this;
				}

			bool lockTry() const noexcept
				{return m_mtx.lockTry();}

			const Waveform& unlock() const noexcept
				{
				m_mtx.unlock();
				return *this;
				}

			bool recorded() const noexcept
				{return m_flags&RECORDED;}

			bool resampled() const noexcept
				{return m_flags&RESAMPLED;}

			Waveform& recorded(bool status) noexcept
				{
				if(status)
					{
					m_flags|=RECORDED|DIRTY;
					m_flags&=~RESAMPLED;
					m_length_ratio=1.0;
					}
				else
					{
					m_flags|=DIRTY;
					m_flags&=~RECORDED;
					}
				return *this;
				}

		private:
			ArrayDynamicShort<float> m_data;
			ArrayFixed<int32_t,4> m_offsets;
			int32_t m_channel;
			float m_gain;
			float m_gain_random;
			double m_fs;
			double m_length_ratio;
			mutable uint32_t m_flags;

			static constexpr uint32_t RECORDED=0x20000000;
			static constexpr uint32_t RESAMPLED=0x40000000;
			static constexpr uint32_t DIRTY=0x80000000;

			template<class T>
			static inline T clamp(T a,T b,T x) noexcept
				{
				if(a<=b)
					{return std::max(a,std::min(x,b));}
				return clamp(b,a,x);
				}
			mutable Mutex m_mtx;

			typedef void (*progress_callback)(void*,Waveform&,float);


			Waveform& load(const SessionFileRecord& rec,const char* filename,progress_callback cb
				,void* cb_obj)
				{
				Mutex::LockGuardNonblocking lock(m_mtx);
				*this=Waveform(rec,filename,cb,cb_obj);
				return *this;
				}

			explicit Waveform(const SessionFileRecord& record,const char* filename
				,progress_callback cb,void* cb_obj);

			Waveform& waveformLoad(const char* filename,progress_callback cb,void* cb_obj);
			Waveform& resample(double fs,progress_callback cb,void* cb_obj);
		};
	}
#endif
