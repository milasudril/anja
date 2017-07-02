//@	{
//@  "targets":[{"name":"button.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"button.o","rel":"implementation"}]
//@	}

#ifndef ANJA_BUTTON_HPP
#define ANJA_BUTTON_HPP

#include <memory>

namespace Anja
	{
	class Container;

	class Button
		{
		public:
			explicit Button(Container& container,const char* label);
			~Button();

			Button& operator=(Button&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Button(Button&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			template<class Callback,class IdType>
			Button& callback(Callback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,Button& self)
					{
					auto x=reinterpret_cast<Callback*>(rvc);
					auto id=static_cast<IdType>(self.id());
					x->clicked(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id));
				}

			const char* label() const noexcept;

			Button& label(const char* text);

			int id() const noexcept;

			Button& state(bool s) noexcept;

			bool state() const noexcept;

		protected:
			class Impl;
			explicit Button(Impl& impl):m_impl(&impl){}
			Impl* m_impl;
			typedef void (*Callback)(void* cb_obj,Button& self);
			Button& callback(Callback cb,void* cb_obj,int id);
		};
	}

#endif // ANJA_RANGEVIEW_HPP
