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

			enum class RunStatus:int{CONTINUE,WAIT};

			template<class IdleCallback>
			void run(IdleCallback& t)
				{
				auto cb_wrapper=[](void* cb_obj,UiContext& self)
					{
					auto cb=reinterpret_cast<IdleCallback*>(cb_obj);
					return cb->idle(self);
					};
				run(cb_wrapper,&t);
				}

		private:
			class Impl;
			Impl* m_impl;
			UiContext(Impl& impl):m_impl(&impl){}
			typedef RunStatus (*IdleCallbackImpl)(void* cb_obj,UiContext& self);
			void run(IdleCallbackImpl cb,void* cb_obj);
		};
	}

#endif