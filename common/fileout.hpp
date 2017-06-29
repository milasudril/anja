//@	{"targets":[{"name":"fileout.hpp","type":"include"}]}

#ifndef ANJA_FILEOUT_HPP
#define ANJA_FILEOUT_HPP

#include "error.hpp"
#include "syserror.hpp"

namespace Anja
	{
	class FileOut
		{
		public:
			FileOut(const FileOut&)=delete;
			FileOut& operator=(const FileOut&)=delete;

		//I am lazy, and will never need these anyway
			FileOut(FileOut&&)=delete;
			FileOut& operator=(FileOut&&)=delete;

			FileOut(const char* name)
				{
				m_handle=(name==nullptr? stdin : fopen(name,"wb"));
				if(m_handle==NULL)
					{throw Error("It was not possible to open the file ",name,". ",SysError(errno));}
				}

			~FileOut()
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
