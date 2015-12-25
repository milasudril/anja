#ifdef __WAND__
target[name[array_non_owning.h] type[include]]
#endif

#ifndef ARRAYNONOWNING_H
#define ARRAYNONOWNING_H

#include <cstdint>

template<class T>
class ArrayNonOwning
	{
	public:
		constexpr ArrayNonOwning(const T* buffer,uint32_t length):
			r_buffer(buffer),m_length(length)
			{}

		constexpr const T* begin() const
			{return r_buffer;}

		constexpr const T* end() const
			{return r_buffer + m_length;}

		constexpr uint32_t length() const
			{return m_length;}

	private:
		const T* r_buffer;
		uint32_t m_length;
	};

#endif
