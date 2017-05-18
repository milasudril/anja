//@	{
//@	"targets":[{"name":"error.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"error.o","rel":"implementation"}]
//@	}

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
			explicit Error(T ... args) noexcept
				{message_build(m_buffer.begin(),args...);}

			const char* message() const noexcept
				{return m_buffer.begin();}

		private:
			ArrayFixed<char,4096> m_buffer;

			template<class Head,class ... T>
			char* message_build(char* pos,const Head& obj,T ... args)
				{
				pos=append<Head>(obj,pos,m_buffer.end()-1);
				return message_build(pos,args...);
				}

			char* message_build(char* pos)
				{
				*pos=0;
				return pos+1;
				}
		};
	}

#endif
