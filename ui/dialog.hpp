//@	{"targets":[{"name":"dialog.hpp","type":"include"}]}

#ifndef ANJA_DIALOG_HPP
#define ANJA_DIALOG_HPP

#include "window.hpp"
#include "button.hpp"
#include "box.hpp"
#include "filler.hpp"
#include "../common/addmemberif.hpp"
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
	inline std::array<Button,1> buttons_create<1>(Container& cnt)
		{return std::array<Button,1>{Button(cnt,"")};}

	template<>
	inline std::array<Button,2> buttons_create<2>(Container& cnt)
		{return std::array<Button,2>{Button(cnt,""),Button(cnt,"")};}

	template<>
	inline std::array<Button,3> buttons_create<3>(Container& cnt)
		{return std::array<Button,3>{Button(cnt,""),Button(cnt,""),Button(cnt,"")};}


	template<class Widget,class DialogTraits=DialogOkCancel>
	class Dialog
		{
		public:
			enum class ButtonId:int{DISMISS,CONFIRM_NEGATIVE,CONFIRM_POSITIVE};

			Dialog& operator=(Dialog&&)=delete;
			Dialog(Dialog&&)=delete;

			template<class ... WidgetParams>
			Dialog(Container& owner,const char* title,WidgetParams... params):m_window(title,&owner)
				,m_content(m_window,true)
					,m_widget(m_content.insertMode({0,Box::FILL|Box::EXPAND}),params...)
						,m_buttons_outer(m_content.insertMode(Box::InsertMode{0,0}),false)
							,m_filler_l(m_buttons_outer.insertMode({0,Box::FILL|Box::EXPAND}))
							,m_buttons_box(m_buttons_outer.insertMode({0,0}),false)
								,m_buttons(buttons_create<button_count()>(m_buttons_box.homogenous(true).insertMode(Box::InsertMode{2,Box::FILL|Box::EXPAND})))
							,m_filler_r(m_buttons_outer.insertMode({0,Box::FILL|Box::EXPAND}))
				{
				m_window.modal(true).show();
				if(has_dismiss())
					{m_buttons[dismiss_index()].label(DialogTraits::dismiss());}
				if(has_confirm_neg())
					{m_buttons[confirm_neg_index()].label(DialogTraits::confirmNegative());}
				if(has_confirm_pos())
					{m_buttons[confirm_pos_index()].label(DialogTraits::confirmPositive());}
				}

			template<class Callback,class IdType>
			Dialog& callback(Callback& cb_obj,IdType id)
				{
				m_vtable=Vtable(cb_obj,id);
				r_cb_obj=&cb_obj;
				m_id=static_cast<int>(id);
				button_callback_assign<dismiss_index()>(ButtonId::DISMISS);
				button_callback_assign<confirm_neg_index()>(ButtonId::CONFIRM_NEGATIVE);
				button_callback_assign<confirm_pos_index()>(ButtonId::CONFIRM_POSITIVE);
				m_window.callback(*this,0);
				return *this;
				}

			Widget& widget() noexcept
				{return m_widget;}

			const Widget& widget() const noexcept
				{return m_widget;}

			void clicked(Button& button,ButtonId id)
				{
				button.state(0);
				switch(id)
					{
					case ButtonId::DISMISS:
						m_vtable.dismiss(r_cb_obj,*this,m_id);
						break;
					case ButtonId::CONFIRM_NEGATIVE:
						m_vtable.confirm_negative(r_cb_obj,*this,m_id);
						break;
					case ButtonId::CONFIRM_POSITIVE:
						m_vtable.confirm_positive(r_cb_obj,*this,m_id);
						break;
					}
				//	We are dead now
				}

			void closing(Window& win,int id)
				{
				if(has_dismiss())
					{m_vtable.dismiss(r_cb_obj,*this,m_id);}
				else
					{m_vtable.confirm_positive(r_cb_obj,*this,m_id);}
				}

			void keyDown(Window& win,int scancode,keymask_t keymask,int id)
				{
				switch(scancode)
					{
					case 1: //ESC
						if(has_dismiss())
							{m_vtable.dismiss(r_cb_obj,*this,m_id);}
						else
							{m_vtable.confirm_positive(r_cb_obj,*this,m_id);}
						break;
					case 28:
						m_vtable.confirm_positive(r_cb_obj,*this,m_id);
						break;
					default:
						break;
					}
				}

			void keyUp(Window& win,int scancode,keymask_t keymask,int id)
				{}

		private:
			static constexpr bool has_dismiss()
				{return DialogTraits::dismiss()!=nullptr;}

			static constexpr bool has_confirm_neg()
				{return DialogTraits::confirmNegative()!=nullptr;}

			static constexpr bool has_confirm_pos()
				{return DialogTraits::confirmPositive()!=nullptr;}

			static constexpr int button_count() noexcept
				{
				return static_cast<int>(has_dismiss())
					+ static_cast<int>(has_confirm_neg())
					+ static_cast<int>(has_confirm_pos());
				}

/*
GNU/Linux: No, Cancel, Yes
MacOS: No, Cancel, Yes
Windows: Yes, No, Cancel
*/

			static constexpr int dismiss_index() noexcept
				{
				if(!has_dismiss())
					{return -1;}
			#ifdef _WIN32
				return button_count()-1;
			#else
				return 0;
			#endif
				}

			static constexpr int confirm_neg_index() noexcept
				{
				if(!has_confirm_neg())
					{return -1;}
			#ifdef _WIN32
				return has_confirm_pos()?1:0;
			#else
				return has_dismiss()?1:0;
			#endif
				}

			static constexpr int confirm_pos_index() noexcept
				{
				if(!has_confirm_pos())
					{return -1;}
			#ifdef _WIN32
				return 0;
			#else
				return button_count() - 1;
			#endif
				}

			template<int index,bool dummy=true>
			struct button_callback_assign_do
				{
				static void assign(Dialog& dlg,ButtonId id)
					{dlg.m_buttons[index].callback(dlg,id);}
				};

			template<bool dummy>
			struct button_callback_assign_do<-1,dummy>
				{
				static void assign(Dialog& dlg,ButtonId id)
					{}
				};

			template<int index>
			void button_callback_assign(ButtonId id)
				{button_callback_assign_do<index>::assign(*this,id);}

			typedef void (*DialogCallback)(void* cb_obj,Dialog& self,int id);

			class Vtable
				{
				public:
					Vtable(){}
					template<class Callback,class IdType>
					explicit Vtable(Callback& cb,IdType) noexcept
						{
						m_dismiss.value(dismiss_call<Callback,IdType,has_dismiss()>::callback);
						m_confirm_negative.value(confirm_negative_call<Callback,IdType,has_confirm_neg()>::callback);
						m_confirm_positive.value(confirm_positive_call<Callback,IdType,has_confirm_pos()>::callback);
						}

					void dismiss(void* cb_obj,Dialog& dlg,int id)
						{m_dismiss.value()(cb_obj,dlg,id);}

					void confirm_negative(void* cb_obj,Dialog& dlg,int id)
						{m_confirm_negative.value()(cb_obj,dlg,id);}

					void confirm_positive(void* cb_obj,Dialog& dlg,int id)
						{m_confirm_positive.value()(cb_obj,dlg,id);}

				private:
					AddMemberIf<has_dismiss(),DialogCallback > m_dismiss;
					AddMemberIf<has_confirm_neg(),DialogCallback> m_confirm_negative;
					AddMemberIf<has_confirm_pos(),DialogCallback> m_confirm_positive;

					template<class Callback,class IdType,bool enable>
					struct dismiss_call
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{reinterpret_cast<Callback*>(cb_obj)->dismiss(self,static_cast<IdType>(id));}
						};

					template<class Callback,class IdType>
					struct dismiss_call<Callback,IdType,0>
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{}
						};

					template<class Callback,class IdType,bool enable>
					struct confirm_negative_call
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{reinterpret_cast<Callback*>(cb_obj)->confirmNegative(self,static_cast<IdType>(id));}
						};

					template<class Callback,class IdType>
					struct confirm_negative_call<Callback,IdType,0>
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{}
						};

					template<class Callback,class IdType,bool enable>
					struct confirm_positive_call
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{reinterpret_cast<Callback*>(cb_obj)->confirmPositive(self,static_cast<IdType>(id));}
						};

					template<class Callback,class IdType>
					struct confirm_positive_call<Callback,IdType,0>
						{
						static void callback(void* cb_obj,Dialog& self,int id)
							{}
						};
				};

			int m_id;
			void* r_cb_obj;
			Vtable m_vtable;

			Window m_window;
				Box m_content;
					Widget m_widget;
					Box m_buttons_outer;
						Filler m_filler_l;
						Box m_buttons_box;
							std::array<Button,button_count()> m_buttons;
						Filler m_filler_r;
		};
	}

#endif
