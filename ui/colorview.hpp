//@	{
//@	 "targets":[{"name":"colorview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"colorview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_COLORVIEW_HPP
#define ANJA_COLORVIEW_HPP

#include <utility>

namespace Anja
	{
	class Container;
	class ColorRGBA;

	class ColorView
		{
		public:
			explicit ColorView(Container& cnt);
			~ColorView();

			ColorView& operator=(ColorView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ColorView(ColorView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ColorView& color(const ColorRGBA& color);

			const ColorRGBA& color() const noexcept;

			template<class Callback,class IdType>
			ColorView& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,ColorView& source)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(source,static_cast<IdType>(source.id()));};
				return callback(cb,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

		private:
			class Impl;
			Impl* m_impl;

			ColorView(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,ColorView& self);
			ColorView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // ANJA_COLORVIEW_HPP
