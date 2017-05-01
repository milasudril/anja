//@	{
//@	 "targets":[{"name":"checkbox.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"checkbox.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CHECKBOX_HPP
#define ANJA_CHECKBOX_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class Checkbox
		{
		public:
			explicit Checkbox(Container& cnt,int id,const char* title);
			~Checkbox();


			Checkbox& operator=(Checkbox&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Checkbox(Checkbox&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			bool state() const noexcept;
			Checkbox& state(bool state_new);
			int id() const noexcept;

			template<class Callback>
			Checkbox& callback(Callback& cb)
				{
				auto cb_wrapper=[](void* rvc,Checkbox& self)
					{
					auto x=reinterpret_cast<Callback*>(rvc);
					x->clicked(self);
					};
				return callback(cb_wrapper,&cb); 
				}

		private:
			class Impl;
			explicit Checkbox(Impl& impl):m_impl(&impl){}
			Impl* m_impl;
			typedef void (*Callback)(void* cb_obj,Checkbox& self);
			Checkbox& callback(Callback cb,void* cb_obj);
		};
	}

#endif
