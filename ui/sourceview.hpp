//@	{
//@	 "targets":[{"name":"sourceview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sourceview.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SOURCEVIEW_HPP
#define ANJA_SOURCEVIEW_HPP

#include <utility>
#include <cstddef>

namespace Anja
	{
	class Container;

	class SourceView
		{
		public:
			explicit SourceView(Container& cnt);
			~SourceView();

			SourceView& operator=(SourceView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			SourceView(SourceView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}


			SourceView& lineNumbers(bool status);

			SourceView& highlight(const char* filename_pattern);

			const char* content() const;

			SourceView& content(const char* text);

			template<class SourceViewCallback,class IdType>
			SourceView& callback(SourceViewCallback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,SourceView& self)
					{
					auto x=reinterpret_cast<SourceViewCallback*>(rvc);
					auto id=static_cast<IdType>(self.id());
					x->changed(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id));
				}

			int id() const noexcept;

			SourceView& wordwrap(bool status);

		protected:
			class Impl;
			Impl* m_impl;
			explicit SourceView(Impl& impl):m_impl(&impl){}

			typedef void (*Callback)(void* cb_obj,SourceView& self);
			SourceView& callback(Callback cb,void* cb_obj,int id);
		};
	}
#endif
