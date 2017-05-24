//@	{"targets":[{"name":"addmemberif.hpp","type":"include"}]}

#ifndef ANJA_ADDMEMBERIF_HPP
#define ANJA_ADDMEMBERIF_HPP

namespace Anja
	{
	template<bool cond,class T,int tag>
	class AddMemberIf
		{
		public:
			void value(const T&){}
			T value() const noexcept
				{return T{};}
		};

	template<class T,int tag>
	struct AddMemberIf<true,T,tag>
		{
		public:
			typedef T type;
			const T& value() const noexcept
				{return m_value;}
			void value(const T& val)
				{m_value=val;}
		private:
			type m_value;
		};
	}

#endif
