//@	 {"targets":[{"name":"arraysimple.hpp","type":"include"}]}

#ifndef ANJA_ARRAYSIMPLE_HPP
#define ANJA_ARRAYSIMPLE_HPP

#include <cstddef>
#include <utility>
#include <algorithm>

namespace Anja
	{
	template<class T>
	class alignas(2*sizeof(T*)) ArraySimple
		{
		public:
			ArraySimple(const ArraySimple&)=delete;
			ArraySimple& operator=(const ArraySimple&)=delete;

			ArraySimple(ArraySimple&& obj):m_data(obj.m_data),m_N(obj.m_N)
				{
				obj.m_data=nullptr;
				obj.m_N=0;
				}

			ArraySimple(const T* begin,const T* end):m_data(nullptr)
				{
				ArraySimple temp(end-begin);
				std::copy(begin,end,temp.begin());
				*this=std::move(temp);
				}

			ArraySimple& operator=(ArraySimple&& obj)
				{
				std::swap(obj.m_data,m_data);
				std::swap(obj.m_N,m_N);
				return *this;
				}

			explicit ArraySimple(size_t N):m_data(new T[N]),m_N(N)
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

		private:
			T* m_data;
			size_t m_N;
		};
	}
#endif
