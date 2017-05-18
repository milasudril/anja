//@	{"targets":[{"name":"arraydynamicshort.hpp","type":"include"}]}

#ifndef ANJA_ARRAYDYNAMICSHORT_HPP
#define ANJA_ARRAYDYNAMICSHORT_HPP

#include "vectortype.hpp"
#include "arrayinit.hpp"
#include "memoryalloc.hpp"
#include "error.hpp"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <cassert>
#include <new>

namespace Anja
	{
	/**\brief Class representing arrays that can grow.
	 *
	 * This class describes an array type that can grow. Like ArraySimple, the
	 * memory allocation is done through the functions declared in memoryalloc.h
	 * , which makes it possible to use SSE instructions on its elements.
	 *
	 */
	template<class T>
	class ArrayDynamicShort
		{
		public:
			/**\brief Default constructor.
			 *
			 * This is the default constructor. When the array is initialized
			 * , it is empty.
			*/
			ArrayDynamicShort() noexcept
				{m_content.x=vec4_t<int32_t>{0,0,0,0};}

			/**\brief Move constructor.
			*/
			ArrayDynamicShort(ArrayDynamicShort&& obj) noexcept
				{
				m_content.x=obj.m_content.x;
				obj.m_content.x=vec4_t<int32_t>{0,0,0,0};
				}

			/**\brief Move assignment operator.
			*/
			ArrayDynamicShort& operator=(ArrayDynamicShort&& obj) noexcept
				{
				assert(this!=&obj);
				std::swap(m_content.x,obj.m_content.x);
				return *this;
				}

			/**\brief Copy assignment operator.
			 *
			 * This is the copy assignment operator.
			 *
			 * \note Copy assignment requires that T objects can be copy constructed.
			 *
			 */
			ArrayDynamicShort& operator=(const ArrayDynamicShort& obj)
				{
				assert(this!=&obj);
				ArrayDynamicShort<T> temp(obj);
				*this=std::move(temp);
				return *this;
				}

			/**\brief Copy constructor.
			 *
			 * This is the copy constructor.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 */
			ArrayDynamicShort(const ArrayDynamicShort& obj):m_content{0,0,0,0}
				{
				append(obj.begin(),obj.length());
				}

			~ArrayDynamicShort()
				{
				clear();
				memoryFree(m_content.data.pointer);
				}


			/**\brief Returns the length of the array.
			*/
			uint32_t length() const noexcept
				{return m_content.data.length;}

			/**\brief Returns a pointer to the first elemenet in the array.
			*/
			T* begin() noexcept
				{return m_content.data.pointer;}

			/**\brief Returns a pointer to the first elemenet in the array.
			*/
			const T* begin() const noexcept
				{return m_content.data.pointer;}

			/**\brief Returns a pointer to the end of the array.
			*/
			T* end() noexcept
				{return m_content.data.pointer+length();}

			/**\brief Returns a pointer to the end of the array.
			*/
			const T* end() const noexcept
				{return m_content.data.pointer+length();}

			/**\brief Returns a reference to the object at <var>position</var>.
			 */
			T& operator[](uint32_t position) noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			/**\brief Returns a reference to the object at <var>position</var>.
			 */
			const T& operator[](uint32_t position) const noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}


			/**\brief Appends another array object to the array.
			 *
			 * This function appends another array object to the array.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
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

			/**\brief Appends an object to the array.
			 *
			 * This function appends an object to the array.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
			ArrayDynamicShort& append(const T& obj)
				{
				auto length_new=static_cast<size_t>( length() ) + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(obj);
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			/**\brief  Appends an object to the array by move.
			 *
			 * This function appends an object to the array by moving it.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
			ArrayDynamicShort& append(T&& obj)
				{
				auto length_new=length() + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(std::move(obj));
				m_content.data.length=length_new;
				return *this;
				}

			/**\brief  Removes the last element from the array.
			 *
			 * This function removes the last element from the array. It has the
			 * same effect as
			 *
			 * \code{.cpp}
			 * truncate(1);
			 * \endcode
			 *
			 */
			ArrayDynamicShort& truncate()
				{
				assert(length()!=0);
				ArrayInit::destroy(end()-1,end());
				--m_content.data.length;
				return *this;
				}

			/**\brief  Removes the <var>N</var> last element from the array.
			 */
			ArrayDynamicShort& truncate(uint32_t N)
				{
				assert(length() >= N);
				ArrayInit::destroy(end()-N,end());
				m_content.data.length-=N;
				return *this;
				}

			/**\brief Removes all elements from the array.
			 *
			 * This function removes all elements from the array. It has the
			 * same effect as
			 *
			 * \code{.cpp}
			 * truncate(length());
			 * \endcode
			 *
			 */
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


#if ( _WIN32 && (__amd64 || __x86_64 || __x86_64__ || __amd64__) )
			constexpr size_t nextpow2(size_t N)
				{
				return 1<<(8*sizeof(N) - 1 - __builtin_clzll(N) + 1);
				}
#else
			constexpr size_t nextpow2(size_t N)
				{
				return 1<<(8*sizeof(N) - 1 - __builtin_clzl(N) + 1);
				}
#endif

			void resize(size_t N);
			uint32_t capacity() const noexcept
				{return m_content.data.capacity;}
		};


	template<class T>
	void ArrayDynamicShort<T>::resize(size_t N)
		{
		auto N_2=nextpow2(N);
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
