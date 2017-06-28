//@	{
//@  "targets":[{"name":"scrolledwindow.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scrolledwindow.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SCROLLEDWINDOW_HPP
#define ANJA_SCROLLEDWINDOW_HPP

#include "container.hpp"
#include "rectangle.hpp"
#include <utility>
#include <cstdint>

namespace Anja
	{
	class ScrolledWindow:public Container
		{
		public:
			explicit ScrolledWindow(Container& parent);
			~ScrolledWindow();

			ScrolledWindow& operator=(ScrolledWindow& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ScrolledWindow(ScrolledWindow&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ScrolledWindow& add(void* handle);
			ScrolledWindow& show();
			ScrolledWindow& sensitive(bool val);
			void* toplevel() const;
			ScrolledWindow& border(bool val);
			static constexpr uint32_t VERTICAL=1;
			static constexpr uint32_t HORIZONTAL=2;
			ScrolledWindow& directions(uint32_t direction_flags);
			Rectangle boundingBox() const noexcept;

		protected:
			class Impl;
			Impl* m_impl;

			explicit ScrolledWindow(Impl& impl):m_impl(&impl){}
		};
	}

#endif
