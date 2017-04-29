//@	{
//@  "targets":[{"name":"textentry.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"textentry.o","rel":"implementation"}]
//@	}

#ifndef ANJA_TEXTENTRY_HPP
#define ANJA_TEXTENTRY_HPP

#include <utility>

namespace Anja
	{
	class Container;

	class TextEntry
		{
		public:
			explicit TextEntry(Container& container,int id);
			~TextEntry();

			TextEntry& operator=(TextEntry&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TextEntry(TextEntry&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class EntryCallback>
			TextEntry& callback(EntryCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,TextEntry& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					x->changed(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			const char* content() const noexcept;

			TextEntry& content(const char* text);

			TextEntry& width(int n) noexcept;

			TextEntry& small(bool status) noexcept;

			TextEntry& alignment(float x) noexcept;

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,TextEntry& self);
			TextEntry& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
			explicit TextEntry(Impl& impl):m_impl(&impl){}
		};
	}

#endif // ANJA_RANGEVIEW_HPP
