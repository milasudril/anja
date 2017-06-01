//@	 {"targets":[{"name":"arraysimple.hpp","type":"include"}]}

#ifndef ANJA_ARRAYSIMPLE_HPP
#define ANJA_ARRAYSIMPLE_HPP

#include "vectortype.hpp"
#include "memoryalloc.hpp"
#include "arrayinit.hpp"
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cassert>

namespace Anja
	{
	template<class T,class SizeType=size_t>
	class ArraySimple
		{
		public:
			~ArraySimple();

			ArraySimple() noexcept:m_content{0,0}
				{}

			explicit ArraySimple(const T* begin,const T* end);

			explicit ArraySimple(SizeType n_elems);

			template<class Initializer>
			explicit ArraySimple(SizeType n_elems,Initializer&& initializer);


			ArraySimple(const ArraySimple& a);

			ArraySimple& operator=(const ArraySimple& a)
				{
				assert( &a != this);
				ArraySimple<T> temp(a);
				*this=std::move(temp);
				return *this;
				}

			ArraySimple(ArraySimple&& a) noexcept
				{
				m_content.x=a.m_content.x;
				a.m_content.x=TwoPointers{0,0};
				}

			ArraySimple& operator=(ArraySimple&& a) noexcept
				{
 				assert(&a != this);
				std::swap(m_content.x,a.m_content.x);
				return *this;
				}

			T* begin() noexcept
				{return m_content.fields.data;}


			const T* begin() const noexcept
				{return m_content.fields.data;}

			T* end() noexcept
				{return m_content.fields.data+m_content.fields.N;}

			const T* end() const noexcept
				{return m_content.fields.data+m_content.fields.N;}


			SizeType length() const noexcept
				{return m_content.fields.N;}

			const T& operator[](size_t k) const noexcept
				{
				assert(k<m_content.fields.N);
				return m_content.fields.data[k];
				}

			T& operator[](size_t k) noexcept
				{
				assert(k<m_content.fields.N);
				return m_content.fields.data[k];
				}


		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			typedef vec2_t<int64_t> TwoPointers;
#else
			typedef vec2_t<int32_t> TwoPointers;
#endif
			union
				{
				TwoPointers x;
				struct
					{
					T* data;
					SizeType N;
					} fields;
				} m_content;
		};


	template<class T,class SizeType>
	ArraySimple<T,SizeType>::ArraySimple(SizeType n_elems)
		{
		assert(n_elems!=0);
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::create(data,data+n_elems);}
		catch(...)
			{
			memoryFree(data);
			throw;
			}
		m_content.fields.N=n_elems;
		m_content.fields.data=data;
		}

	template<class T,class SizeType>
	template<class Initializer>
	ArraySimple<T,SizeType>::ArraySimple(SizeType n_elems,Initializer&& init)
		{
		assert(n_elems!=0);
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::createFromInitializer(data,data+n_elems,init);}
		catch(...)
			{
			memoryFree(data);
			throw;
			}
		m_content.fields.N=n_elems;
		m_content.fields.data=data;
		}

	template<class T,class SizeType>
	ArraySimple<T,SizeType>::~ArraySimple()
		{
		ArrayInit::destroy(begin(),end());
		memoryFree(m_content.fields.data);
		}


	template<class T,class SizeType>
	ArraySimple<T,SizeType>::ArraySimple(const ArraySimple& a)
		{
		assert(a.begin()!=nullptr);
		auto data=reinterpret_cast<T*>( memoryAllocate(a.length()*sizeof(T)) );
		try
			{ArrayInit::copy(data,data+a.length(),a.begin());}
		catch(...)
			{
			memoryFree(data);
			throw;
			}
		m_content.fields.N=a.length();
		m_content.fields.data=data;
		}

	template<class T,class SizeType>
	ArraySimple<T,SizeType>::ArraySimple(const T* b,const T* e)
		{
		auto l=e - b;
		auto data=reinterpret_cast<T*>( memoryAllocate(l*sizeof(T)) );
		try
			{ArrayInit::copy(data,data+l,b);}
		catch(...)
			{
			memoryFree(data);
			throw;
			}
		m_content.fields.N=l;
		m_content.fields.data=data;
		}
	}
#endif
