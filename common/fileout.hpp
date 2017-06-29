//@	{"targets":[{"name":"fileout.hpp","type":"include"}]}

#ifndef ANJA_FILEOUT_HPP
#define ANJA_FILEOUT_HPP

#include "error.hpp"

namespace Anja
	{
	class FileOut
		{
		public:
			FileOut(const FileIn&)=delete;
			FileOut& operator=(const FileIn&)=delete;

		//I am lazy, and will never need these anyway
			FileOut(FileIn&&)=delete;
			FileOut& operator=(FileIn&&)=delete;

			FileOut(const char* name)
				{
				m_handle=(name==nullptr? stdin : fopen(name,"wb"));
				if(m_handle==NULL)
					{throw Anja::Error("Failed to open the file ",name);}
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
