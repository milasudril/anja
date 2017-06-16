//@	{
//@  "targets":[{"name":"slider.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"slider.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SLIDER_HPP
#define ANJA_SLIDER_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class Slider
		{
		public:
			explicit Slider(Container& container,bool vertical);
			~Slider();

			Slider& operator=(Slider&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Slider(Slider&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class EntryCallback,class IdType>
			Slider& callback(EntryCallback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,Slider& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					auto id=static_cast<IdType>( self.id() );
					x->changed(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id)); 
				}

			double value() const noexcept;

			Slider& value(double x);

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,Slider& self);
			Slider& callback(Callback cb,void* cb_obj,int id);

			class Impl;
			Impl* m_impl;
			explicit Slider(Impl& impl):m_impl(&impl){}
		};
	}

#endif
