#ifdef __WAND__
target[name[ringbuffer.h] type[include]]
#endif

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "framework/array_simple.h"

#include <cstdint>

template<class T>
class RingBuffer:private ArraySimple<T>
	{
	public:
		using ArraySimple<T>::length;

		explicit RingBuffer(size_t N):ArraySimple<T>(N)
			,m_offset_read(0),m_offset_write(0)
			{}

		void push_back(T x)
			{
			auto owr=m_offset_write;
			uint32_t N=ArraySimple<T>::m_N;
			ArraySimple<T>::m_data[owr]=x;
			owr=(owr+1)%N;
			m_offset_write=owr;
			}

		T pop_front()
			{
			auto ore=m_offset_read;
			auto ret=ArraySimple<T>::m_data[ore];
			uint32_t N=ArraySimple<T>::m_N;
			m_offset_read=(ore+1)%N;
			return ret;
			}

		bool full() const
			{
			return m_offset_write < m_offset_read;
			}

		bool empty() const
			{
			return m_offset_read==m_offset_write;
			}

	private:
		uint32_t m_offset_read;
		uint32_t m_offset_write;
	};

#endif
