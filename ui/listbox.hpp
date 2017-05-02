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

			Listbox& append(const char* option);

			Listbox& replace(int index,const char* option);

			Listbox& selected(int index) noexcept;

			int selected() const noexcept;
			
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

		private:
			typedef void (*Callback)(void* cb_obj,Listbox& self);
			Listbox& callback(Callback cb,void* cb_obj,int id);
			class Impl;
			Impl* m_impl;
			Listbox(Impl& impl):m_impl(&impl){}
		};
	}

#endif