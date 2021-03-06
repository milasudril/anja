//@	{
//@	"targets":[{"type":"include","name":"keyboardview.hpp"}]
//@	,"dependencies_extra":[{"rel":"implementation","ref":"keyboardview.o"}]
//@	}

#ifndef ANJA_KEYBOARDVIEW_HPP
#define ANJA_KEYBOARDVIEW_HPP

#include "../common/color.hpp"
#include <string>

namespace Anja
	{
	class Container;
	class KeyboardView
		{
		public:
			explicit KeyboardView(Container& cnt);
			~KeyboardView();

			KeyboardView& operator=(KeyboardView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			KeyboardView(KeyboardView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			template<class Callback,class IdType>
			KeyboardView& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,KeyboardView& source)
					{reinterpret_cast<Callback*>(cb_obj)->indexSelected(source,static_cast<IdType>(source.id()));};
				return callback(cb,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

			int selection() const noexcept;

			KeyboardView& selection(int scancode);

			KeyboardView& keyColor(int scancode,const ColorRGBA& color);

			const ColorRGBA& keyColor(int scancode) const noexcept;

			KeyboardView& keyLabel(int scancode,const char* label);

			const char* keyLabel(int scancode) const noexcept;


			KeyboardView& redraw();

			enum class KeyType:int{TYPING_KEY,FUNCTION_KEY,OTHER};

			std::pair<KeyType,int> keyType(int scancode) const noexcept;

			bool modifier(int scancode) const noexcept;

			KeyboardView& reset();

	private:
		class Impl;
		Impl* m_impl;

		KeyboardView(Impl& impl):m_impl(&impl)
			{}

		typedef void (*CallbackImpl)(void* cb_obj,KeyboardView& self);
		KeyboardView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif
