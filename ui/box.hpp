//@	{
//@  "targets":[{"name":"box.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"box.o","rel":"implementation"}]
//@	}

#ifndef ANJA_BOX_HPP
#define ANJA_BOX_HPP

#include "container.hpp"
#include <utility>

namespace Anja
	{
	class Box:public Container
		{
		public:
			static constexpr unsigned short FILL=1;
			static constexpr unsigned short EXPAND=2;
			struct InsertMode
				{
				unsigned short padding;
				unsigned short flags;
				};

			explicit Box(Container& parent,bool vertical);
			~Box();

			Box& operator=(Box&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Box(Box&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Box& add(void* handle);
			Box& show();
			Box& sensitive(bool val);
			void* toplevel() const;

			Box& homogenous(bool status) noexcept;
			Box& insertMode(const InsertMode& mode) noexcept;

			Box& alignment(float x) noexcept;

		protected:
			class Impl;
			explicit Box(Box::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // ANJA_Box_HPP
