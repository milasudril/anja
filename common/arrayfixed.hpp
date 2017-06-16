//@	{"targets":[{"name":"arrayfixed.hpp","type":"include"}]}

#ifndef ANJA_ARRAYFIXED_HPP
#define ANJA_ARRAYFIXED_HPP

#include <cstddef>

namespace Anja
	{
	template<class T,size_t N>
	class ArrayFixed
		{
		public:
			constexpr const T& operator[](size_t i) const noexcept
				{return data[i];}

			constexpr T& operator[](size_t i) noexcept
				{return data[i];}

			constexpr const T* begin() const noexcept
				{return data;}

			constexpr const T* end() const noexcept
				{return data+N;}

			constexpr T* begin() noexcept
				{return data;}

			constexpr T* end() noexcept
				{return data+N;}

			static constexpr size_t length() noexcept
				{return N;}

			static constexpr size_t sizeBytes() noexcept
				{return N*sizeof(T);}

			constexpr ArrayFixed():data{}
				{}

			template<class ... Vals>
			constexpr ArrayFixed(const T& x,Vals ... v):data{x,v...}
				{}

		private:
			T data[N];
		};
	}
#endif
