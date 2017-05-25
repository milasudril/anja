//@	{
//@	 "targets":[{"name":"imageview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imageview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_IMAGEVIEW_HPP
#define ANJA_IMAGEVIEW_HPP

#include <utility>
#include <cstdint>

namespace Anja
	{
	class Container;

	class ImageView
		{
		public:
			explicit ImageView(Container& cnt);
			~ImageView();

			ImageView& operator=(ImageView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ImageView(ImageView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ImageView& showPng(const uint8_t* bytes_begin,const uint8_t* bytes_end);

			ImageView& minHeight(int h);

			ImageView& minWidth(int w);

			template<class Callback,class IdType>
			ImageView& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,ImageView& source)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(source,static_cast<IdType>(source.id()));};
				return callback(cb,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

		private:
			class Impl;
			Impl* m_impl;

			ImageView(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,ImageView& self);
			ImageView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // ANJA_IMAGEVIEW_HPP
