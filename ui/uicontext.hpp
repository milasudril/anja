//@	{
//@	 "targets":[{"name":"uicontext.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"uicontext.o","rel":"implementation"}]
//@	}

#ifndef ANJA_UICONTEXT_HPP
#define ANJA_UICONTEXT_HPP

#include <cstdint>

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

			template<class Callback>
			void run(Callback& cb)
				{
				Vtable vt
					{
					[](void* cb_obj,UiContext& self)
						{
						auto cb=reinterpret_cast<Callback*>(cb_obj);
						return cb->idle(self);
						}
					,[](void* cb_obj,UiContext& self,int32_t id,int32_t param)
						{
						auto cb=reinterpret_cast<Callback*>(cb_obj);
						cb->process(self,static_cast<typename Callback::MessageId>(id)
							,static_cast<typename Callback::MessageParam>(param));
						}
					};
				run(vt,&cb);
				}

			UiContext& dark(bool status);

			bool messagePostTry(int32_t id,int32_t param) noexcept;

			UiContext& messagePost(int32_t id,int32_t param);

		private:
			class Impl;
			Impl* m_impl;
			struct Vtable
				{
				RunStatus (*idle)(void* cb_obj,UiContext& self);
				void (*process)(void* cb_obj,UiContext& self,int32_t id,int32_t param);
				};

			UiContext(Impl& impl):m_impl(&impl){}
			void run(const Vtable& vt,void* cb_obj);
		};
	}

#endif
