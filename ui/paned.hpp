//@	{
//@  "targets":[{"name":"paned.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paned.o","rel":"implementation"}]
//@	}

#ifndef ANJA_PANED_HPP
#define ANJA_PANED_HPP

#include "container.hpp"
#include "rectangle.hpp"
#include <utility>

namespace Anja
	{
	class Paned:public Container
		{
		public:
			static constexpr unsigned short RESIZE=1;
			static constexpr unsigned short SHRINK_ALLOWED=2;

			struct InsertMode
				{unsigned short flags;};

			explicit Paned(Container& parent,bool vertical);
			~Paned();

			Paned& operator=(Paned&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Paned(Paned&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Paned& add(void* handle);
			Paned& show();
			Paned& sensitive(bool val);
			void* toplevel() const;
			Paned& insertMode(const InsertMode& mode) noexcept;
			Rectangle boundingBox(int index) const noexcept;

		protected:
			class Impl;
			explicit Paned(Paned::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // ANJA_Paned_HPP
