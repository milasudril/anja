//@	{
//@	"targets":[{"name":"error.o","type":"object"}]
//@	}

#include "error.hpp"

namespace Anja
	{
	template<>
	char* append<const char*>(const char* src,char* ptr,char* ptr_max)
		{
		while(ptr!=ptr_max && *src!='\0')
			{
			*ptr=*src;
			++src;
			++ptr;
			}
		return ptr;
		}
	}
