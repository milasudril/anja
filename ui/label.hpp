//@	{
//@  "targets":[{"name":"label.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"label.o","rel":"implementation"}]
//@	}

#ifndef ANJA_LABEL_HPP
#define ANJA_LABEL_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class Label
		{
		public:
			explicit Label(Container& container,const char* content);
			~Label();

			Label& operator=(Label&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Label(Label&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Label& content(const char* text);

			const char* content()const noexcept;

		protected:
			class Impl;
			Impl* m_impl;

			explicit Label(Impl& impl):m_impl(&impl){}
		};
	}

#endif
