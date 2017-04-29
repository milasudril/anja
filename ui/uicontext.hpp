//@	{
//@	 "targets":[{"name":"uicontext.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"uicontext.o","rel":"implementation"}]
//@	}

#ifndef ANJA_UICONTEXT_HPP
#define ANJA_UICONTEXT_HPP

namespace Anja
	{
	class UiContext
		{
		public:
			UiContext(const UiContext&)=delete;
			UiContext& operator=(const UiContext&)=delete;
			UiContext();
			~UiContext();
			void exit();
			void run();
			
			template<class Callback>
			void uiUpdate(Callback& cb)
				{
				auto cb_wrapper=[](void* cb_obj)
					{
					auto obj=reinterpret_cast<Callback*>(cb_obj);
					obj->uiUpdate();
					return 0;
					};
				ui_update(cb_wrapper,&cb);
				}

		private:
			class Impl;
			Impl* m_impl;
			UiContext(Impl& impl):m_impl(&impl){}
			typedef int (*UiUpdate)(void* cb);
			void ui_update(UiUpdate update,void* cb);
		};
	}

#endif