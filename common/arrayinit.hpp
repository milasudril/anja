//@	{"targets":[{"name":"arrayinit.hpp","type":"include"}]}

#ifndef ANJA_ARRAYINIT_HPP
#define ANJA_ARRAYINIT_HPP

#include <new>
#include <cstddef>
#include <type_traits>
#include <cstring>
#include <utility>

namespace Anja
	{
	namespace ArrayInit
		{
		template<class T>
		void destroy(T* begin,T* end)
			{
			if(!std::is_trivially_destructible<T>::value)
				{
				while(end!=begin)
					{
					--end;
					end->~T();
					}
				}
			}

		template<class T,class Template>
		void createFromTemplate(T* begin,T* end,const Template& x)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T(x);
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		template<class T>
		void create(T* begin,T* end)
			{
			if(!std::is_trivially_default_constructible<T>::value)
				{
				auto position=begin;
				try
					{
					while(position!=end)
						{
						new(position)T();
						++position;
						}
					}
				catch(...)
					{
					destroy(begin,position);
					throw;
					}
				}
			}

		template<class T>
		using ElementInitializer = T (*)(T* address,size_t k);


		template<class T,class Initializer>
		void createFromInitializer(T* begin,T* end,Initializer&& init)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					init(position,position-begin);
					++position;
					}
				}
			catch(...)
				{
				destroy(begin,position);
				throw;
				}
			}

		template<class T,class U>
		void copy(T* begin,T* end,const U* source)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T(*source);
					++source;
					++position;
					}
				}
			catch(...)
				{
				destroy(begin,position);
				throw;
				}
			}

		template<class T>
		size_t copy(T* begin,T* end,const T* source)
			{
			auto N=end-begin;
			if(std::is_trivially_copy_constructible<T>::value)
				{
				memcpy(begin,source,sizeof(T)*N);
				}
			else
				{copy<T,T>(begin,end,source);}
			return N;
			}
		}
	}

#endif
