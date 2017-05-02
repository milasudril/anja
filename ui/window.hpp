//@	{
//@  "targets":[{"name":"window.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"window.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WINDOW_HPP
#define ANJA_WINDOW_HPP

#include "container.hpp"
#include <utility>

namespace Anja
	{
	class Window:public Container
		{
		public:
			explicit Window(const char* title);
			~Window();

			Window& operator=(Window&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Window(Window&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Window& add(void* handle);
			Window& show();
			Window& sensitive(bool val);
			void* toplevel() const;

			const char* title() const noexcept;
			Window& title(const char* title_new);

			template<class WindowCallback,class IdType>
			Window& callback(WindowCallback& cb,IdType id) noexcept
				{
				auto cb_wrapper=[](void* wc,Window& self)
					{
					auto x=reinterpret_cast<WindowCallback*>(wc);
					auto id=static_cast<IdType>(self.id());
					x->closing(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id)); 
				}

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,Window& self);
			Window& callback(Callback cb,void* cb_obj,int id);

			class Impl;
			Impl* m_impl;
			explicit Window(Impl& impl):m_impl(&impl){}
		};
	}

#endif
