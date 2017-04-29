//@	{
//@  "targets":[{"name":"tabview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"tabview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_TABVIEW_HPP
#define ANJA_TABVIEW_HPP

#include "container.hpp"
#include <utility>

namespace Anja
	{
	class TabView:public Container
		{
		public:
			explicit TabView(Container& parent);
			~TabView();

			TabView& operator=(TabView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TabView(TabView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			TabView& add(void* handle);
			TabView& show();
			TabView& sensitive(bool val);
			TabView& tabTitle(const char* t);
			void* toplevel() const;

			TabView& activate(int index) noexcept;

		protected:
			class Impl;
			explicit TabView(TabView::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // ANJA_TabView_HPP
