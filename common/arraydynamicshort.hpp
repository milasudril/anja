//@	{"targets":[{"name":"arraydynamicshort.hpp","type":"include"}]}

#ifndef ANJA_ARRAYDYNAMICSHORT_HPP
#define ANJA_ARRAYDYNAMICSHORT_HPP

#include "vectortype.hpp"
#include "arrayinit.hpp"
#include "memoryalloc.hpp"
#include "error.hpp"
#include "nextpow2.hpp"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <cassert>
#include <new>

namespace Anja
	{
	template<class T>
	class ArrayDynamicShort
		{
		public:
			ArrayDynamicShort() noexcept __attribute__((always_inline))
				{m_content.x=vec4_t<int32_t>{0,0,0,0};}

			ArrayDynamicShort(ArrayDynamicShort&& obj) noexcept
				{
				m_content.x=obj.m_content.x;
				obj.m_content.x=vec4_t<int32_t>{0,0,0,0};
				}

			ArrayDynamicShort& operator=(ArrayDynamicShort&& obj) noexcept
				{
				assert(this!=&obj);
				std::swap(m_content.x,obj.m_content.x);
				return *this;
				}

			ArrayDynamicShort& operator=(const ArrayDynamicShort& obj)
				{
				assert(this!=&obj);
				ArrayDynamicShort<T> temp(obj);
				*this=std::move(temp);
				return *this;
				}


			ArrayDynamicShort(const ArrayDynamicShort& obj):m_content{0,0,0,0}
				{
				append(obj.begin(),obj.length());
				}

			~ArrayDynamicShort()
				{
				clear();
				memoryFree(m_content.data.pointer);
				}


			uint32_t length() const noexcept
				{return m_content.data.length;}

			T* begin() noexcept
				{return m_content.data.pointer;}

			const T* begin() const noexcept
				{return m_content.data.pointer;}


			T* end() noexcept
				{return m_content.data.pointer+length();}


			const T* end() const noexcept
				{return m_content.data.pointer+length();}

			T& operator[](uint32_t position) noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			const T& operator[](uint32_t position) const noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			ArrayDynamicShort& append(const T* begin_in,uint32_t length_in)
				{
				assert(begin_in!=begin());
				auto length_new=static_cast<size_t>( length() )+length_in;
				if(capacity() < length_new + 1)
					{resize(length_new + 1);}
				ArrayInit::copy(end(),end()+length_in,begin_in);
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			ArrayDynamicShort& append(const T& obj)
				{
				auto length_new=static_cast<size_t>( length() ) + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(obj);
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			ArrayDynamicShort& append(T&& obj)
				{
				auto length_new=length() + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(std::move(obj));
				m_content.data.length=length_new;
				return *this;
				}

			ArrayDynamicShort& truncate()
				{
				assert(length()!=0);
				ArrayInit::destroy(end()-1,end());
				--m_content.data.length;
				return *this;
				}

			ArrayDynamicShort& truncate(uint32_t N)
				{
				assert(length() >= N);
				ArrayInit::destroy(end()-N,end());
				m_content.data.length-=N;
				return *this;
				}

			ArrayDynamicShort& clear()
				{return truncate(length());}

			ArrayDynamicShort& capacity(uint32_t n)
				{
				if(n > m_content.data.capacity)
					{resize(n);}
				return *this;
				}

		private:
			union
				{
				vec4_t<int32_t> x;
				struct
					{
					T* pointer;
					uint32_t length;
					uint32_t capacity;
					} data;
				} m_content;

			void resize(size_t N);
			uint32_t capacity() const noexcept
				{return m_content.data.capacity;}
		};


	template<class T>
	void ArrayDynamicShort<T>::resize(size_t N)
		{
		auto N_2=Nextpow2<size_t>(N);
		if(N_2 > std::numeric_limits<uint32_t>::max())
			{
			throw Error("Data does not fit in the array.");
			}

		T* block_new=reinterpret_cast<T*>(memoryRealloc(begin(),N_2*sizeof(T)));
		m_content.data.pointer=block_new;
		m_content.data.capacity=static_cast<uint32_t>(N_2);
		}
	}

#endif
