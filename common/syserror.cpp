//@	{
//@	"targets":[{"name":"syserror.o","type":"object"}]
//@}

#include "error.hpp"
#include "syserror.hpp"

#ifdef _GNU_SOURCE
extern "C"
	{
#warning "Default strerror_r is not POSIX complient. Your vendor should fix this."
	extern int __xpg_strerror_r (int __errnum, char *__buf, size_t __buflen);
	#define strerror_r __xpg_strerror_r
	extern size_t strlen(const char* s);
	}
#else
#include <string.h>
#endif

namespace Anja
	{
	template<>
	char* append<SysError>(SysError e,char* ptr,char* ptr_max)
		{
		strerror_r(e.status(),ptr,ptr_max-ptr);
		return ptr+strlen(ptr);
		}
	}
