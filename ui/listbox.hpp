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
			explicit Listbox(Container& parent,int id);
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

			template<class EntryCallback>
			Listbox& callback(EntryCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,Listbox& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					x->changed(self);
					};
				return callback(cb_wrapper,&cb); 
				}

		private:
			typedef void (*Callback)(void* cb_obj,Listbox& self);
			Listbox& callback(Callback cb,void* cb_obj);
			class Impl;
			Impl* m_impl;
			Listbox(Impl& impl):m_impl(&impl){}
		};
	}

#endif