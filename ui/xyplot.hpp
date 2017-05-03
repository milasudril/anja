//@	{
//@	"targets":[{"name":"xyplot.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"xyplot.o","rel":"implementation"}]
//@	}

#ifndef ANJA_XYPLOT_HPP
#define ANJA_XYPLOT_HPP

#include "keymask.hpp"
#include <utility>

namespace Anja
	{
	class Container;
	class XYPlot
		{
		public:
			explicit XYPlot(Container& cnt);
			~XYPlot();

			XYPlot& operator=(XYPlot&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			XYPlot(XYPlot&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			int id();

			template<class Callback,class IdType>
			XYPlot& callback(Callback& cb,IdType id) noexcept
				{return callback(Vtable(cb,id),&cb,static_cast<int>(id));}

			struct Point
				{
				double x;
				double y;
				};

			XYPlot& curve(const Point* begin,const Point* end,float hue);

			struct Domain
				{
				Point min;
				Point max;
				};

			XYPlot& domain(const Domain& dom) noexcept;		
			Domain domain() const noexcept;

			struct Cursor
				{
				double position;
				float hue;
				};

			XYPlot& cursorX(const Cursor& c);
			XYPlot& cursorX(const Cursor& c,int index);
			Cursor cursorX(int index) const noexcept;

			XYPlot& cursorY(const Cursor& c);
			XYPlot& cursorY(const Cursor& c,int index);
			Cursor cursorY(int index) const noexcept;

			XYPlot& showAll() noexcept;

		private:
			class Impl;
			Impl* m_impl;
			XYPlot(Impl& impl) noexcept:m_impl(&impl){}

			struct Vtable
				{
				Vtable() noexcept;

				template<class Callback,class IdType>
				explicit Vtable(Callback& cb,IdType) noexcept
					{
					mouse_move=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseMove(source,static_cast<IdType>(source.id()),x,keymask);};
					mouse_down=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseDown(source,static_cast<IdType>(source.id()),x,keymask);};
					mouse_up=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseUp(source,static_cast<IdType>(source.id()),x,keymask);};
					key_down=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyDown(source,static_cast<IdType>(source.id()),scancode);};
					key_up=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyUp(source,static_cast<IdType>(source.id()),scancode);};
					key_down=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyDown(source,static_cast<IdType>(source.id()),scancode);};
					cursor_x=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorX(source,static_cast<IdType>(source.id()),index,keymask);};
					cursor_y=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorY(source,static_cast<IdType>(source.id()),index,keymask);};
					}

				void (*mouse_move)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*mouse_down)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*mouse_up)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*key_down)(void* cb_obj,XYPlot& source,uint8_t scancode);
				void (*key_up)(void* cb_obj,XYPlot& source,uint8_t scancode);
				void (*cursor_x)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				void (*cursor_y)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				};

			XYPlot& callback(const Vtable& vt,void* cb_obj,int id);
		};
	}

#endif // ANJA_XYPLOT_HPP
