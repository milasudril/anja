//@	{
//@	 "targets":[{"name":"colorview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"colorview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_COLORVIEW_HPP
#define ANJA_COLORVIEW_HPP

#include <utility>

namespace Anja
	{
	class Container;
	class ColorRGBA;

	class ColorView
		{
		public:
			explicit ColorView(Container& cnt);
			~ColorView();

			ColorView& operator=(ColorView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ColorView(ColorView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ColorView& color(const ColorRGBA& color);


		private:
			class Impl;
			Impl* m_impl;

			ColorView(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,ColorView& self);
			ColorView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // ANJA_COLORVIEW_HPP
