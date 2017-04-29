//@	{
//@	 "dependencies_extra":[{"ref":"string.o","rel":"implementation"}]
//@	,"targets":[{"name":"string.hpp","type":"include"}]
//@	}
#ifndef ANJA_STRING_HPP
#define ANJA_STRING_HPP

#include "arraydynamicshort.hpp"

namespace Anja
	{
	class String:private ArrayDynamicShort<char>
		{
		public:
			using ArrayDynamicShort<char>::begin;
			using ArrayDynamicShort<char>::end;

			const char* end() const noexcept
				{return ArrayDynamicShort<char>::end()-1;}

			char* end() noexcept
				{return ArrayDynamicShort<char>::end()-1;}

			unsigned int length() const noexcept
				{return ArrayDynamicShort<char>::length()-1;}


			String()
				{ArrayDynamicShort<char>::append('\0');}

			explicit String(const char* c_str)
				{
				ArrayDynamicShort<char>::append('\0');
				append(c_str);
				}

			const char* beginAfter(char ch) const noexcept;

			char* beginAfter(char ch) noexcept;


			String& append(const char* c_str)
				{
				assert(c_str!=begin());
				return appendImpl(c_str);
				}

			String& append(const String& str)
				{
				ArrayDynamicShort<char>::truncate().append(str);
				return *this;
				}

			String& append(char ch)
				{
				ArrayDynamicShort<char>::truncate().append(ch).append('\0');
				return *this;
				}

			String& truncate() noexcept
				{
				ArrayDynamicShort<char>::truncate();
				(*end())='\0';
				return *this;
				}

			String& truncate(char ch) noexcept;

			String& clear() noexcept
				{
				ArrayDynamicShort<char>::clear();
				ArrayDynamicShort<char>::append('\0');
				return *this;
				}

		private:
			String& appendImpl(const char* c_str);
		};

		bool operator==(const String& a,const char* b) noexcept;
		bool operator==(const String& a,const String& b) noexcept;

		inline bool operator!=(const String& a,const String& b) noexcept
			{return !(a==b);}

		inline bool operator!=(const String& a,const char* b) noexcept
			{return !(a==b);}
	}

#endif
