#ifndef ANJA_RINGBUFFER_HPP
#define ANJA_RINGBUFFER_HPP

#include "arraysimple.hpp"

#include <cstdint>

namespace Anja
	{
	template<class T>
	class RingBuffer:private ArraySimple<T>
		{
		public:
			using ArraySimple<T>::length;

			explicit RingBuffer(size_t N):ArraySimple<T>(N)
				,m_offset_read(0),m_offset_write(0)
				{}

			void push_back(const T& x)
				{
				auto owr=m_offset_write;
				ArraySimple<T>::m_data[owr]=x;
				uint32_t N=ArraySimple<T>::m_N;
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
			volatile uint32_t m_offset_read;
			volatile uint32_t m_offset_write;
		};
	}

#endif
