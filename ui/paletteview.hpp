//@	{
//@	 "targets":[{"name":"paletteview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paletteview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_PALETTEVIEW_HPP
#define ANJA_PALETTEVIEW_HPP

#include <utility>
#include <cstddef>

namespace Anja
	{
	class Container;
	class ColorRGBA;

	class PaletteView
		{
		public:
			explicit PaletteView(Container& cnt);
			~PaletteView();

			PaletteView& operator=(PaletteView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			PaletteView(PaletteView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			PaletteView& palette(const ColorRGBA* colors,size_t n);

		private:
			class Impl;
			Impl* m_impl;

			PaletteView(Impl& impl):m_impl(&impl)
				{}
		};
	}

#endif // ANJA_PALETTEVIEW_HPP
