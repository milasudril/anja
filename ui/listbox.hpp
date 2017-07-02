//@	{
//@	"targets":[{"name":"listbox.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"listbox.o","rel":"implementation"}]
//@	}

#ifndef ANJA_LISTBOX_HPP
#define ANJA_LISTBOX_HPP

#include <utility>

namespace Anja
	{
	class Container;
	class Listbox
		{
		public:
			explicit Listbox(Container& parent);
			~Listbox();

			Listbox& operator=(Listbox&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Listbox(Listbox&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			int id() const noexcept;

			Listbox& append(const char* item);

			Listbox& selected(int index,bool status) noexcept;

			bool selected(int index) const noexcept;

			const char* label(int index);

			Listbox& clear() noexcept;

			template<class EntryCallback,class IdType>
			Listbox& callback(EntryCallback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,Listbox& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					auto id=static_cast<IdType>(self.id());
					x->changed(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id));
				}

			template<class EnumCallback>
			const Listbox& itemsEnum(EnumCallback&& cb) const noexcept
				{
				auto cb_obj=&cb;
				auto cb_wrapper=[](void* cb,Listbox& self,int index)
					{
					auto obj=reinterpret_cast<EnumCallback*>(cb);
					return (*obj)(self,index);
					};

				return itemsEnum(cb,cb_wrapper);
				}

		private:
			typedef void (*Callback)(void* cb_obj,Listbox& self);
			Listbox& callback(Callback cb,void* cb_obj,int id);
			class Impl;
			Impl* m_impl;
			Listbox(Impl& impl):m_impl(&impl){}

			const Listbox& itemsEnum(void* cb,bool (*enum_callback)(void* cb,Listbox& self,int index));
		};
	}

#endif
