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
	class OptionListImpl
		{
		public:
			explicit OptionListImpl(Container& cnt,bool vertical):
				m_scroll(cnt)
					,m_box_main(m_scroll,vertical)
						,m_box(m_box_main,vertical)
				{
				m_scroll.border(1);
				m_box.homogenous(1).insertMode({0,Box::EXPAND|Box::FILL});
				}

			OptionListImpl& clear() noexcept
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

			OptionListImpl& append(const char* text)
				{
				m_options.push_back(Checkbox(m_box,text));
				return *this;
				}

			OptionListImpl& append(const char* const* labels);

			Checkbox& back() noexcept
				{return m_options.back();}

			size_t size() const noexcept
				{return m_options.size();}

			OptionListImpl& selected(uint64_t mask);

		private:
			ScrolledWindow m_scroll;
				Box m_box_main;
					Box m_box;
					
			std::vector<Checkbox> m_options;
		};

	template<class Callback,class IdType>
	class OptionList
		{
		public:
			explicit OptionList(Container& cnt,bool vertical):r_callback(nullptr)
				,m_impl(cnt,vertical)
				{}

			OptionList& clear() noexcept
				{
				m_impl.clear();	
				return *this;
				}

			OptionList& append(const char* text)
				{
				m_impl.append(text);
				return *this;
				}

			OptionList& callback(Callback& cb,IdType id) noexcept
				{
				r_callback=&cb;
				m_id=id;
				int k=0;
				std::for_each(m_impl.begin(),m_impl.end()
					,[this,&k](Checkbox& btn)
						{
						btn.callback(*this,k);
						++k;
						});

				return *this;
				}

			void clicked(Checkbox& btn,int id)
				{r_callback->clicked(*this,btn,id);}
			
			IdType id() const noexcept
				{return m_id;}

			auto begin() noexcept
				{return m_impl.begin();}

			auto end() noexcept
				{return m_impl.end();}

			Checkbox& operator[](int k) noexcept
				{return m_impl[k];}

			OptionList& append(const char* const* labels)
				{
				m_impl.append(labels);
				return *this;
				}

			size_t size() const noexcept
				{return m_impl.size();}

			OptionList& selected(uint64_t mask)
				{
				m_impl.selected(mask);
				return *this;
				}

		private:
			IdType m_id;
			Callback* r_callback;
			OptionListImpl m_impl;
		};
	}

#endif // ANJA_BUTTONLIST_HPP
