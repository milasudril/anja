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
			explicit TextEntry(Container& container);
			~TextEntry();

			TextEntry& operator=(TextEntry&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TextEntry(TextEntry&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class EntryCallback,class IdType>
			TextEntry& callback(EntryCallback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,TextEntry& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					auto id=static_cast<IdType>(self.id());
					x->changed(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id)); 
				}

			const char* content() const noexcept;

			TextEntry& content(const char* text);

			TextEntry& width(int n) noexcept;

			TextEntry& small(bool status) noexcept;

			TextEntry& alignment(float x) noexcept;

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,TextEntry& self);
			TextEntry& callback(Callback cb,void* cb_obj,int id);

			class Impl;
			Impl* m_impl;
			explicit TextEntry(Impl& impl):m_impl(&impl){}
		};
	}

#endif // ANJA_RANGEVIEW_HPP
