//@	{"targets":[{"name":"wavetable.hpp","type":"include"}]}

#ifndef ANJA_ARRAYFIXED_H
#define ANJA_ARRAYFIXED_H

#include <cstddef>

template<class T,size_t N>
class ArrayFixed
	{
	public:
		constexpr const T& operator[](size_t i) const
			{return data[i];}

		T& operator[](size_t i)
			{return data[i];}

		constexpr const T* begin() const
			{return data;}

		constexpr const T* end() const
			{return data+N;}

		T* begin()
			{return data;}

		T* end()
			{return data+N;}

		static constexpr size_t length()
			{return N;}

		static constexpr size_t size()
			{return N*sizeof(T);}

		constexpr ArrayFixed() noexcept:data{}
			{}

		template<class ... Vals>
		constexpr ArrayFixed(const T& x,Vals ... v) noexcept:data{x,v...}
			{}

	private:
		T data[N];
	};

#endif
