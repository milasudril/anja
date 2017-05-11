//@	{
//@	    "targets":[{"name":"vectortype.hpp","type":"include"}]
//@	}

/**\file vectortype.hpp \brief Defines data types for enabling vectorized code.
 */

#ifndef ANJA_VECTORTYPE_H
#define ANJA_VECTORTYPE_H

#ifdef DOXYGEN
#define __attribute__(x)
#define vector_type(Type) vector_type<Type>
#else
#define vector_type(Type) Type
#endif

namespace Anja
	{
#ifdef DOXYGEN
	/**\brief class used to define types that uses vectorized instruction sets
	 * like MMX and SSE.
	 *
	 * This class is used to define types that uses vectorized instruction sets
	 * like MMX and SSE. With such instruction sets, the compiler may emit
	 * instructions that operates on multiple data at ones.
	 *
	 *  \param T is the component type of the vector. `T` must be a primitive type
	 *           supported by the targeted instruction set.
	 *
	 *  \warning In order for these types to work properly, the vector_type object
	 * must be aligned properly. To avoid alignment problems, use memoryAllocate(size_t)
	 * and memoryFree(void*) in favour of malloc and free.
	 */
	template<class T> class vector_type;
#endif

	/**\brief Vector type with four components.
	 *
	 * This type represents a vector with four components. For example,
	 * \code
	 * Anja::vec4_t<int> a{1,2,3,4};
	 * Anja::vec4_t<int> b{4,3,2,1};
	 * auto c=a+b;
	 * Anja::logWrite(Log::MessageType::INFORMATION,"c=[%d,%d,%d,%d]"
	 *     ,c[0],c[1],c[2],c[3]);
	 * \endcode
	 * will print
	 *
	 *     (i) c=[5,5,5,5]
	 *
	 * to the console.
	 */
	template<class T>
	using vec4_t __attribute__ ((vector_size(4*sizeof(T))))=vector_type(T);

	/**\brief Vector type with two components.
	 *
	 * This type represents a vector with four components. For example,
	 * \code
	 * Anja::vec2_t<int> a{1,2};
	 * Anja::vec2_t<int> b{4,3};
	 * auto c=a+b;
	 * Anja::logWrite(Log::MessageType::INFORMATION,"c=[%d,%d]",c[0],c[1]);
	 * \endcode
	 * will print
	 *
	 *     (i) c=[5,5]
	 *
	 * to the console.
	 */
	template<class T>
	using vec2_t __attribute__ ((vector_size(2*sizeof(T))))=vector_type(T);
	}

#endif
