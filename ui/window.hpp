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
			explicit Window(const char* title,int id);
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

			template<class WindowCallback>
			Window& callback(WindowCallback& cb) noexcept
				{
				auto cb_wrapper=[](void* wc,Window& self)
					{
					auto x=reinterpret_cast<WindowCallback*>(wc);
					x->closing(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,Window& self);
			Window& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
			explicit Window(Impl& impl):m_impl(&impl){}
		};
	}

#endif
