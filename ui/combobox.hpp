//@	{
//@	"targets":[{"name":"combobox.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"combobox.o","rel":"implementation"}]
//@	}

#ifndef ANJA_COMBOBOX_HPP
#define ANJA_COMBOBOX_HPP

#include <utility>

namespace Anja
	{
	class Container;
	class Combobox
		{
		public:
			explicit Combobox(Container& parent);
			~Combobox();

			Combobox& operator=(Combobox&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Combobox(Combobox&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			int id() const noexcept;

			Combobox& append(const char* option);

			Combobox& replace(int index,const char* option);

			Combobox& selected(int index) noexcept;

			int selected() const noexcept;

			Combobox& clear() noexcept;

			template<class EntryCallback,class IdType>
			Combobox& callback(EntryCallback& cb,IdType id)
				{
				auto cb_wrapper=[](void* rvc,Combobox& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					auto id=static_cast<IdType>(self.id());
					x->changed(self,id);
					};
				return callback(cb_wrapper,&cb,static_cast<int>(id));
				}

		private:
			typedef void (*Callback)(void* cb_obj,Combobox& self);
			Combobox& callback(Callback cb,void* cb_obj,int id);
			class Impl;
			Impl* m_impl;
			Combobox(Impl& impl):m_impl(&impl){}
		};
	}

#endif
