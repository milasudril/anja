#ifndef ANJA_RINGBUFFER_HPP
#define ANJA_RINGBUFFER_HPP

#include "arraysimple.hpp"
#include "nextpow2.hpp"

namespace Anja
	{
	template<class T>
	class RingBuffer
		{
		public:
			typedef T value_type;

			explicit RingBuffer(size_t N):
				m_offset_read(0),m_offset_write(0),m_data(Nextpow2<size_t>(N))
				{static_assert(sizeof(T)<=sizeof(size_t),"Type not compatible");}

			void push_back(T x) noexcept
				{
				assert(!full());
				auto owr=m_offset_write;
				m_data[owr%capacity()]=x;
				++m_offset_write;
				}

			T pop_front() noexcept
				{
				assert(!empty());
				auto ore=m_offset_read;
				auto ret=m_data[ore%capacity()];
				++m_offset_read;
				return ret;
				}

			bool full() const noexcept
				{return m_offset_write < m_offset_read;}

			bool empty() const noexcept
				{return m_offset_read==m_offset_write;}

			Nextpow2<size_t> capacity() const noexcept
				{return m_data.length();}

		private:
			volatile uint32_t m_offset_read;
			volatile uint32_t m_offset_write;
			ArraySimple< T,Nextpow2<size_t> > m_data;
		};
	}

#endif
