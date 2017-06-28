//@	{
//@	 "targets":[{"name":"buttonlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"buttonlist.o","rel":"implementation"}]
//@	}

#ifndef ANJA_BUTTONLIST_HPP
#define ANJA_BUTTONLIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "button.hpp"
#include "separator.hpp"
#include <vector>
#include <algorithm>

namespace Anja
	{
	class ButtonList
		{
		public:
			ButtonList& operator=(ButtonList&&)=delete;
			ButtonList(ButtonList&&)=delete;

			explicit ButtonList(Container& cnt,bool vertical):
				m_scroll(cnt)
					,m_box_main(m_scroll,vertical)
						,m_box(m_box_main,vertical)
				{
				m_scroll.border(0);
				m_box.homogenous(!vertical).insertMode({2,Box::EXPAND|Box::FILL});
				}

			ButtonList& clear() noexcept
				{
				m_buttons.clear();
				return *this;
				}

			auto begin() noexcept
				{return m_buttons.begin();}

			auto end() noexcept
				{return m_buttons.end();}

			Button& operator[](int k) noexcept
				{return m_buttons[k];}

			ButtonList& append(const char* text)
				{
				if(*text=='\0')
					{m_separators.push_back(Separator(m_box,false));}
				else
					{m_buttons.push_back(Button(m_box,text));}
				return *this;
				}

			ButtonList& append(const char* const* labels);

			template<class ... T>
			ButtonList& append(const char* head,T ... label)
				{
				append(head);
				return append(label...);
				}

			Button& back() noexcept
				{return m_buttons.back();}

			size_t size() const noexcept
				{return m_buttons.size();}

			ButtonList& selected(uint64_t mask);

			template<class Callback,class IdType>
			ButtonList& callback(Callback& cb,IdType id) noexcept
				{
				m_id=static_cast<int>( id );
				m_cb=[](void* cb_obj,ButtonList& self,int id,Button& option)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(self,static_cast<IdType>(id),option);};
				r_cb_obj=&cb;
				callbacks_assign();
				return *this;
				}

			void clicked(Button& btn,int id)
				{m_cb(r_cb_obj,*this,m_id,btn);}

			Rectangle boundingBox() const noexcept
				{return m_scroll.boundingBox();}

		private:
			int m_id;
			void (*m_cb)(void* cb_obj,ButtonList& self,int id,Button& option);
			void* r_cb_obj;

			ScrolledWindow m_scroll;
				Box m_box_main;
					Box m_box;

			std::vector<Separator> m_separators;
			std::vector<Button> m_buttons;
			void callbacks_assign();
		};
	}

#endif // ANJA_BUTTONLIST_HPP
