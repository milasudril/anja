//@	{
//@  "targets":[{"name":"separator.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"separator.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SEPARATOR_HPP
#define ANJA_SEPARATOR_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class Separator
		{
		public:
			explicit Separator(Container& container,bool vertical) noexcept;
			~Separator();

			Separator& operator=(Separator&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Separator(Separator&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

		protected:
			class Impl;
			Impl* m_impl;
			explicit Separator(Impl& impl):m_impl(&impl){}
		};
	}

#endif
