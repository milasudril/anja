//@	{
//@	"targets":[{"name":"memoryalloc.o","type":"object"}]
//@	}

#include "memoryalloc.hpp"
#include <cstdlib>
#include <cstdint>

using namespace Anja;


void* Anja::memoryAllocate(size_t N)
	{
	static_assert(sizeof(void*)>=8,"Current implementation requires a 64 bit system");
	return malloc(N);
	}

void Anja::memoryFree(void* buffer)
	{
	free(buffer);
	}

void* Anja::memoryRealloc(void* buffer,size_t N)
	{
	static_assert(sizeof(void*)>=8,"Current implementation requires a 64 bit system");
	return realloc(buffer,N);
	}
