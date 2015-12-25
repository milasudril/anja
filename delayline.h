#ifdef __WAND__
target[name[delayline.h] type[include]]
#endif

#ifndef DELAYLINE_H
#define DELAYLINE_H

#include "framework/array_simple.h"
#include <cstdint>

template<class T>
class DelayLine:private ArraySimple<T>
	{
	public:
		using ArraySimple<T>::length;

		explicit DelayLine(size_t N):ArraySimple<T>(N),m_offset_read(0),m_offset_write(N-1)
			{}

		void fill(const T& x)
			{
			uint32_t N=ArraySimple<T>::m_N;
			while(N)
				{
				push_back(x);
				--N;
				}
			}

		void push_back(const T& x)
			{
			auto owr=m_offset_write;
			uint32_t N=ArraySimple<T>::m_N;
			owr=(owr+1)%N;
			ArraySimple<T>::m_data[owr]=x;
			m_offset_write=owr;
			m_offset_read=(owr+1)%N;
			}

		const T& front() const
			{
			return ArraySimple<T>::m_data[m_offset_read];
			}

	private:
		uint32_t m_offset_read;
		uint32_t m_offset_write;
	};

#endif
