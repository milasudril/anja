//@	{"targets":[{"name":"filein.hpp","type":"include"}]}

#ifndef ANJA_FILEIN_HPP
#define ANJA_FILEIN_HPP

#include "error.hpp"
#include "syserror.hpp"

#include <cstdio>
#include <cerrno>

namespace Anja
	{
	class FileIn
		{
		public:
			FileIn(const FileIn&)=delete;
			FileIn& operator=(const FileIn&)=delete;

		//I am lazy, and will never need these anyway
			FileIn(FileIn&&)=delete;
			FileIn& operator=(FileIn&&)=delete;

			FileIn(const char* name)
				{
				m_handle=(name==nullptr? stdin : fopen(name,"rb"));
				if(m_handle==NULL)
					{throw Error("It was not possible to open the file ",name,". ",SysError(errno));}
				}

			~FileIn()
				{
				if(m_handle!=stdin)
					{fclose(m_handle);}
				}

			FILE* get() noexcept
				{return m_handle;}

		private:
			FILE* m_handle;
		};
	}

#endif
