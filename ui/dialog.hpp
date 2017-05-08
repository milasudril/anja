//@	{"targets":[{"name":"dialog.hpp","type":"include"}]}

#ifndef ANJA_DIALOG_HPP
#define ANJA_DIALOG_HPP

#include "window.hpp"
#include "button.hpp"
#include "box.hpp"

#include <array>

namespace Anja	
	{
	struct DialogOkCancel
		{
		static constexpr const char* dismiss() noexcept
			{return "Cancel";}

		static constexpr const char* confirmPositive() noexcept
			{return "Ok";}

		static constexpr const char* confirmNegative() noexcept
			{return nullptr;}
		};

	struct DialogOk
		{
		static constexpr const char* dismiss() noexcept
			{return nullptr;}

		static constexpr const char* confirmPositive() noexcept
			{return "Ok";}

		static constexpr const char* confirmNegative() noexcept
			{return nullptr;}
		};

	template<size_t N>
	std::array<Button,N> buttons_create(Container& cnt);

	template<>
	std::array<Button,1> buttons_create<1>(Container& cnt)
		{return std::array<Button,1>{Button(cnt,"")};}

	template<>
	std::array<Button,2> buttons_create<2>(Container& cnt)
		{return std::array<Button,2>{Button(cnt,""),Button(cnt,"")};}

	template<>
	std::array<Button,3> buttons_create<3>(Container& cnt)
		{return std::array<Button,3>{Button(cnt,""),Button(cnt,""),Button(cnt,"")};}

	template<class Widget,class DialogTrait=DialogOkCancel>
	class Dialog
		{
		public:
			enum class ButtonId:int{DISMISS,CONFIRM_NEGATIVE,CONFIRM_POSITIVE};

			Dialog(Container& owner,const char* title):m_window(title,&owner)
				,m_content(m_window,true)
					,m_widget(m_content.insertMode(Box::InsertMode{0,Box::FILL|Box::EXPAND}))
					,m_buttons_box(m_content.insertMode(Box::InsertMode{0,0}),false)
						,m_buttons(buttons_create<button_count()>(m_buttons_box.homogenous(true).insertMode(Box::InsertMode{0,Box::EXPAND})))
				{
				m_window.modal(true).show();
				if(DialogTrait::dismiss()!=nullptr)
					{m_buttons[dismiss_index()].label(DialogTrait::dismiss());}
				if(DialogTrait::confirmNegative()!=nullptr)
					{m_buttons[confirm_neg_index()].label(DialogTrait::confirmNegative());}
				if(DialogTrait::confirmPositive()!=nullptr)
					{m_buttons[confirm_pos_index()].label(DialogTrait::confirmPositive());}
				}

			template<class Callback,class IdType>
			Dialog& callback(Callback& cb_obj,IdType id)
				{
				m_vtable=Vtable(cb_obj);
				m_id=id;
				if(DialogTrait::dismiss()!=nullptr)
					{m_buttons[dismiss_index()].callback(*this,ButtonId::DISMISS);}
				if(DialogTrait::confirmNegative()!=nullptr)
					{m_buttons[confirm_neg_index()].callback(*this,ButtonId::CONFIRM_NEGATIVE);}
				if(DialogTrait::confirmPositive()!=nullptr)
					{m_buttons[confirm_pos_index()].callback(*this,ButtonId::CONFIRM_POSITIVE);}
				return *this;
				}

			void clicked(Button& button,ButtonId id)
				{
				switch(id)
					{
					case ButtonId::DISMISS:
						break;
					case ButtonId::CONFIRM_NEGATIVE:
						break;
					case ButtonId::CONFIRM_POSITIVE:
						break;
					}
				button.state(0);
				}

		private:
			static constexpr int button_count() noexcept
				{
				return static_cast<int>(DialogTrait::dismiss()!=nullptr)
					+ static_cast<int>(DialogTrait::confirmNegative()!=nullptr)
					+ static_cast<int>(DialogTrait::confirmPositive()!=nullptr);
				}

/*
GNU/Linux: No, Cancel, Yes
MacOS: No, Cancel, Yes
Windows: Yes, No, Cancel
*/
			
			static constexpr int dismiss_index() noexcept
				{
			#ifdef _WIN32
				return button_count()-1;
			#else
				return 0;
			#endif
				}

			static constexpr int confirm_neg_index() noexcept
				{
			#ifdef _WIN32
				return DialogTrait::confirmPositive()==nullptr?0:1;
			#else
				return DialogTrait::dismiss()==nullptr?0:1;
			#endif
				}

			static constexpr int confirm_pos_index() noexcept
				{
			#ifdef _WIN32
				return 0;
			#else
				return button_count() - 1;
			#endif
				}

			struct Vtable
				{
				Vtable():dismiss(nullptr),confirm_negative(nullptr)
					,confirm_positive(nullptr)
					{}
			
				template<class Callback>
				Vtable(Callback& cb)
					{}

				void (*dismiss)();
				void (*confirm_negative)();
				void (*confirm_positive)();
				};

			Vtable m_vtable;
			int m_id;

			Window m_window;
				Box m_content;
					Widget m_widget;
					Box m_buttons_box;
						std::array<Button,button_count()> m_buttons;
		};
	}

#endif
