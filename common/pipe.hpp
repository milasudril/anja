//@	{
//@	 "targets":[{"name":"pipe.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"pipe.o","rel":"implementation"}]
//@	}

#ifndef ANJA_PIPE_HPP
#define ANJA_PIPE_HPP

#include <utility>
#include <cstdint>
#include <cstddef>

namespace Anja
	{

	typedef void (*PipeCallbackFn)(void* cb_obj,const int8_t* buffer,size_t n);
	void pipe(const char* const* command,PipeCallbackFn cb,void* cb_obj);

	template<class PipeCallback>
	void pipe(const char* const* command,PipeCallback&& cb_obj)
		{
		auto cb=[](void* cb_obj,const int8_t* buffer,size_t n)
			{
			reinterpret_cast<PipeCallback*>(cb_obj)->write(buffer,n);
			};
		pipe(command,cb,&cb_obj);
		}

	template<class PipeCallback>
	void pipe(const char* const* command,PipeCallback& cb_obj)
		{
		auto cb=[](void* cb_obj,const int8_t* buffer,size_t n)
			{
			reinterpret_cast<PipeCallback*>(cb_obj)->write(buffer,n);
			};
		pipe(command,cb,&cb_obj);
		}
	}

#endif
