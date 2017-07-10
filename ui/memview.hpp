//@	{
//@	 "targets":[{"name":"memview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"memview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_MEMVIEW_HPP
#define ANJA_MEMVIEW_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class MemView
		{
		public:
			explicit MemView(Container& cnt);
			~MemView();

			MemView& operator=(MemView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			MemView(MemView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

		private:
			class Impl;
			Impl* m_impl;

			MemView(Impl& impl):m_impl(&impl)
				{}
		};
	}

#endif
