//@	{"targets":[{"name":"delayline.hpp","type":"include"}]}

#ifndef ANJA_DELAYLINE_HPP
#define ANJA_DELAYLINE_HPP

#include "arraysimple.hpp"
#include <cstdint>

namespace Anja
	{
	template<class T>
	class DelayLine:private ArraySimple<T>
		{
		public:
			int32_t length() const noexcept
				{return static_cast<int32_t>( ArraySimple<T>::length() );}

			explicit DelayLine(int32_t N):ArraySimple<T>(N),m_offset_read(0),m_offset_write(N-1)
				{}

			void fill(const T& x)
				{
				auto N=length();
				while(N)
					{
					push_back(x);
					--N;
					}
				}

			void push_back(const T& x)
				{
				auto owr=m_offset_write;
				auto N=length();
				owr=(owr+1)%N;
				ArraySimple<T>::operator[](owr)=x;
				m_offset_write=owr;
				m_offset_read=(owr+1)%N;
				}

			const T& front() const
				{
				return ArraySimple<T>::operator[](m_offset_read);
				}

		private:
			int32_t m_offset_read;
			int32_t m_offset_write;
		};
	}
#endif
