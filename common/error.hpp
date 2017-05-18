//@	{"targets":[{"name":"error.hpp","type":"include"}]}

#ifndef ANJA_ERROR_HPP
#define ANJA_ERROR_HPP

#include "arrayfixed.hpp"

namespace Anja
	{
	template<class T>
	char* append(T src,char* ptr,char* ptr_max);

	class Error
		{
		public:
			template<class ... T>
			Error(T ... args) noexcept
				{message_build(m_buffer.begin(),args...);}

		private:
			ArrayFixed<char,4096> m_buffer;

			template<class Head,class ... T>
			char* message_build(char* pos,const Head& obj,T ... args)
				{
				pos=append<Head>(obj,pos,m_buffer.end());
				return message_build(pos,args...);
				}

			char* message_build(char* pos)
				{return pos;}
		};

	template<>
	char* append<const char*>(const char* src,char* ptr,char* ptr_max);
	}

#endif
