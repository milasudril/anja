#ifdef __WAND__
target[name[array_fixed.h] type[include]]
#endif

#ifndef ARRAYFIXED_H
#define ARRAYFIXED_H

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

		constexpr size_t length()
			{return N;}

		constexpr size_t size()
			{return N*sizeof(T);}

		T data[N];
	};

#endif
