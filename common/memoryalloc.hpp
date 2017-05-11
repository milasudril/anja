//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"memoryalloc.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"memoryalloc.hpp",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}

/**\file memoryalloc.hpp \brief Memory allocation routines.
 *
 * The memory allocation routines declared in this file are SSE safe.
 */

#ifndef ANJA_ALLOC_HPP
#define ANJA_ALLOC_HPP

#include <cstddef>

namespace Anja
	{
	/**\brief Allocates N bytes from the heap.
	 *
	 * This function allocates N bytes from the heap. If the function fails
	 * to allocate memory, an ErrorMessage is thrown.
	 *
	 * \note The pointer returned is properly aligned so vectorized instructions
	 * can be used.
	 *
	 * \warning Do not use `free` to free the allocated memory. Use memoryFree(void*)
	 * instead.
	*/
	void* memoryAllocate(size_t N);

	/**\brief Frees memory allocated by memoryAllocate(size_t).
	 *
	 * This function frees memory allocated by memoryAllocate(size_t).
	 *
	 * \warning Do not pass a pointer allocated by `malloc` to this function.
	 *
	 */
	void memoryFree(void* buffer);


	/**\brief Reallocates memory allocated by memoryAllocate(size_t).
	 *
	 * This function reallocates memory allocated by memoryAllocate(size_t).
	 *
	 * \warning Do not pass a pointer allocated by `malloc` to this function.
	 *
	 */
	void* memoryRealloc(void* buffer,size_t N);
	}

#endif
