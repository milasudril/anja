//@	{
//@	 "targets":[{"name":"knob.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"knob.o","rel":"implementation"}]
//@	}

#ifndef ANJA_KNOB_HPP
#define ANJA_KNOB_HPP

#include <utility>
#include <cstdint>

namespace Anja
	{
	class Container;

	class Knob
		{
		public:
			explicit Knob(Container& cnt);
			~Knob();

			Knob& operator=(Knob&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Knob(Knob&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			template<class Callback,class IdType>
			Knob& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,Knob& source)
					{reinterpret_cast<Callback*>(cb_obj)->changed(source,static_cast<IdType>(source.id()));};
				return callback(cb,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

			double value() const noexcept;

			Knob& value(double x);

		private:
			class Impl;
			Impl* m_impl;

			Knob(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,Knob& self);
			Knob& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // ANJA_KNOB_HPP
