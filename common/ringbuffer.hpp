#ifndef ANJA_RINGBUFFER_HPP
#define ANJA_RINGBUFFER_HPP

#include "arraysimple.hpp"

#include <cstdint>

namespace Anja
	{
	template<class T>
	class RingBuffer
		{
		public:
			explicit RingBuffer(size_t N):
				m_offset_read(0),m_offset_write(0),m_data(N)
				{}

			void push_back(T x)
				{
				auto owr=m_offset_write;
				m_data[owr]=x;
				uint32_t N=length();
				m_offset_write=(owr+1)%N;
				}

			T pop_front()
				{
				auto ore=m_offset_read;
				auto ret=m_data[ore];
				uint32_t N=length();
				m_offset_read=(ore+1)%N;
				return ret;
				}

			bool full() const
				{return m_offset_write < m_offset_read;}

			bool empty() const
				{return m_offset_read==m_offset_write;}

			uint32_t length() const noexcept
				{return m_data.length();}

		private:
			volatile uint32_t m_offset_read;
			volatile uint32_t m_offset_write;
			ArraySimple<T> m_data;
		};
	}

#endif
