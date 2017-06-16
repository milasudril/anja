//@	{
//@	"targets":[{"name":"memoryalloc.o","type":"object"
//@		,"dependencies":[{"ref":"jemalloc","rel":"external"}]}]
//@	}

#include "memoryalloc.hpp"
#include "error.hpp"
#include <cstdlib>
#include <cstdint>
#include <jemalloc/jemalloc.h>

using namespace Anja;

#ifdef __AVX__
static constexpr int ALIGNMENT=32;
#else
static constexpr int ALIGNMENT=16;
#endif


void* Anja::memoryAllocate(size_t N)
	{
	void* ret=mallocx(N,MALLOCX_ALIGN(ALIGNMENT));
	if(ret==NULL)
		{throw Error("Failed to allocate more memory");}
	return ret;
	}

void Anja::memoryFree(void* buffer)
	{
	if(buffer!=NULL)
		{dallocx(buffer,MALLOCX_ALIGN(ALIGNMENT));}
	}

void* Anja::memoryRealloc(void* buffer,size_t N)
	{
	if(buffer==nullptr)
		{return memoryAllocate(N);}
	void* ret=rallocx(buffer,N,MALLOCX_ALIGN(ALIGNMENT));
	if(ret==nullptr)
		{throw Error("It was not possible to extend current memory block");}
	return ret;
	}
