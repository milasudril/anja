//@	{
//@  "targets":[{"name":"window.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"window.o","rel":"implementation"}]
//@	}

#ifndef ANJA_WINDOW_HPP
#define ANJA_WINDOW_HPP

#include "container.hpp"
#include "keymask.hpp"
#include "imagerepository.hpp"
#include <utility>

namespace Anja
	{
	class Window:public Container
		{
		public:
			explicit Window(const char* title,Container* owner=nullptr);
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
				return callback(Vtable(cb,id),&cb,static_cast<int>(id));
				}

			int id() const noexcept;

			Window& modal(bool state);

			Window& fullscreen(bool state);

			Window& icon(const ImageRepository& repo,ImageRepository::IdType id
				,const std::pair<const uint8_t*,const uint8_t*>& data);

		protected:
			struct Vtable
				{
				Vtable():closing(nullptr),key_down(nullptr),key_up(nullptr)
					{}

				template<class Callback,class IdType>
				Vtable(Callback& cb_obj,IdType id)
					{
					closing=[](void* cb_obj,Window& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->closing(self,static_cast<IdType>(id));};
					key_down=[](void* cb_obj,Window& self,int scancode,keymask_t keymask,int id)
						{reinterpret_cast<Callback*>(cb_obj)->keyDown(self,scancode,keymask,static_cast<IdType>(id));};
					key_up=[](void* cb_obj,Window& self,int scancode,keymask_t keymask,int id)
						{reinterpret_cast<Callback*>(cb_obj)->keyUp(self,scancode,keymask,static_cast<IdType>(id));};
					}

				void (*closing)(void* cb_obj,Window& self,int id);
				void (*key_down)(void* cb_obj,Window& self,int scancode,keymask_t keymask,int id);
				void (*key_up)(void* cb_obj,Window& self,int scancode,keymask_t keymask,int id);
				};


			Window& callback(const Vtable& vt,void* cb_obj,int id);

			class Impl;
			Impl* m_impl;
			explicit Window(Impl& impl):m_impl(&impl){}
		};
	}

#endif
