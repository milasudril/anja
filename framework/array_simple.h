#ifdef __WAND__
target[name[array_simple.h] type[include]]
#endif

#ifndef ARRAYSIMPLE_H
#define ARRAYSIMPLE_H

#include <cstddef>

template<class T>
class ArraySimple
	{
	public:
		ArraySimple(const ArraySimple&)=delete;
		ArraySimple& operator=(const ArraySimple&)=delete;

		ArraySimple(size_t N):m_data(new T[N]),m_N(N)
			{}

		const T* begin() const
			{return m_data;}

		const T* end() const
			{return m_data+m_N;}

		T* begin()
			{return m_data;}

		T* end()
			{return m_data+m_N;}

		size_t length() const
			{return m_N;}

		~ArraySimple()
			{delete[] m_data;}

		const T& operator[](size_t i) const
			{return m_data[i];}

		T& operator[](size_t i)
			{return m_data[i];}

	protected:
		T* m_data;
		size_t m_N;
	};

#endif
