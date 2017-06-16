//@	{
//@	 "targets":[{"name":"optionlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"optionlist.o","rel":"implementation"}]
//@	}

#ifndef ANJA_OPTIONLIST_HPP
#define ANJA_OPTIONLIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "checkbox.hpp"
#include <vector>
#include <algorithm>

namespace Anja
	{
	class OptionList
		{
		public:
			OptionList& operator=(OptionList&&)=delete;
			OptionList(OptionList&&)=delete;

			explicit OptionList(Container& cnt,bool vertical):
				m_scroll(cnt)
					,m_box_main(m_scroll,vertical)
						,m_box(m_box_main,vertical)
				{
				m_scroll.border(1);
				m_box.homogenous(1).insertMode({0,Box::EXPAND|Box::FILL});
				}

			OptionList& clear() noexcept
				{
				m_options.clear();
				return *this;
				}

			auto begin() noexcept
				{return m_options.begin();}

			auto end() noexcept
				{return m_options.end();}

			Checkbox& operator[](int k) noexcept
				{return m_options[k];}

			OptionList& append(const char* text)
				{
				m_options.push_back(Checkbox(m_box,text));
				return *this;
				}

			OptionList& append(const char* const* labels);

			Checkbox& back() noexcept
				{return m_options.back();}

			size_t size() const noexcept
				{return m_options.size();}

			OptionList& selected(uint64_t mask);

			template<class Callback,class IdType>
			OptionList& callback(Callback& cb,IdType id) noexcept
				{
				m_id=static_cast<int>( id );
				m_cb=[](void* cb_obj,OptionList& self,int id,Checkbox& option)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(self,static_cast<IdType>(id),option);};
				r_cb_obj=&cb;
				callbacks_assign();
				return *this;
				}

			void clicked(Checkbox& btn,int id)
				{m_cb(r_cb_obj,*this,m_id,btn);}

		private:
			int m_id;
			void (*m_cb)(void* cb_obj,OptionList& self,int id,Checkbox& option);
			void* r_cb_obj;

			ScrolledWindow m_scroll;
				Box m_box_main;
					Box m_box;

			std::vector<Checkbox> m_options;
			void callbacks_assign();
		};
	}

#endif // ANJA_BUTTONLIST_HPP
