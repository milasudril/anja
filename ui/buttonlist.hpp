//@	{
//@	 "targets":[{"name":"buttonlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"buttonlist.o","rel":"implementation"}]
//@	}

#ifndef ANJA_BUTTONLIST_HPP
#define ANJA_BUTTONLIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "button.hpp"
#include <vector>
#include <algorithm>

namespace Anja
	{
	class ButtonListImpl
		{
		public:
			explicit ButtonListImpl(Container& cnt,int id,bool vertical):m_id(id)
				,m_scroll(cnt)
					,m_box_main(m_scroll,vertical)
						,m_box(m_box_main,vertical)
				{
				m_box.homogenous(1).insertMode({0,Box::EXPAND|Box::FILL});
				}

			ButtonListImpl& clear() noexcept
				{
				m_buttons.clear();
				return *this;
				}

			ButtonListImpl& operator=(ButtonListImpl&&)=delete;
			ButtonListImpl(ButtonListImpl&&)=delete;

			int id() const noexcept
				{return m_id;}

			auto begin() noexcept
				{return m_buttons.begin();}

			auto end() noexcept
				{return m_buttons.end();}

			Button& operator[](int k) noexcept
				{return m_buttons[k];}

			ButtonListImpl& append(const char* text)
				{
				m_buttons.push_back(Button(m_box,text));
				return *this;
				}

			ButtonListImpl& append(const char* const* labels);

			Button& back() noexcept
				{return m_buttons.back();}

			size_t size() const noexcept
				{return m_buttons.size();}

		private:
			int m_id;
			ScrolledWindow m_scroll;
				Box m_box_main;
					Box m_box;

			std::vector<Button> m_buttons;
		};

	template<class Callback,class ButtonId>
	class ButtonList
		{
		public:
			explicit ButtonList(Container& cnt,int id,bool vertical):r_callback(nullptr)
				,m_impl(cnt,id,vertical)
				{}

			ButtonList& clear() noexcept
				{
				m_impl.clear();
				return *this;
				}

			ButtonList& append(const char* text)
				{
				m_impl.append(text);
				return *this;
				}

			ButtonList& callback(Callback& cb) noexcept
				{
				r_callback=&cb;
				int k=0;
				std::for_each(m_impl.begin(),m_impl.end()
					,[this,&k](Button& btn)
						{
						btn.callback(*this,k);
						++k;
						});

				return *this;
				}

			void clicked(Button& btn,int id)
				{r_callback->clicked(*this,btn,static_cast<ButtonId>(id));}

			int id() const noexcept
				{return m_impl.id();}

			auto begin() noexcept
				{return m_impl.begin();}

			auto end() noexcept
				{return m_impl.end();}

			Button& operator[](int k) noexcept
				{return m_impl[k];}

			ButtonList& append(const char* const* labels)
				{
				m_impl.append(labels);
				return *this;
				}

			size_t size() const noexcept
				{return m_impl.size();}

		private:
			Callback* r_callback;
			ButtonListImpl m_impl;
		};
	}

#endif // ANJA_BUTTONLIST_HPP
