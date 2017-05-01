//@	{"targets":[{"name":"xyplot.hpp","type":"include"}]}

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
			explicit XYPlot(Container& cnt,int id);
			~XYPlot();

			XYPlot& operator=(XYPlot&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			XYPlot(XYPlot&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			int id();

			template<class Callback>
			XYPlot& callback(Callback& cb) noexcept
				{return callback(Vtable(cb),&cb);}

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

			XYPlot& cursorX(double position,float hue) noexcept;
			double cursorX(int index) const noexcept;
			XYPlot& cursorY(double position,float hue) noexcept;
			double cursorY(int index) const noexcept;

		private:
			class Impl;
			Impl* m_impl;
			XYPlot(Impl& impl) noexcept:m_impl(&impl){}

			struct Vtable
				{
				template<class Callback>
				explicit Vtable(Callback& cb) noexcept
					{
					mouse_move=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseMove(source,x,keymask);};
					mouse_down=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseDown(source,x,keymask);};
					mouse_up=[](void* cb_obj,XYPlot& source,Point x,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->mouseUp(source,x,keymask);};
					key_down=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyDown(source,scancode);};
					key_up=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyUp(source,scancode);};
					key_down=[](void* cb_obj,XYPlot& source,uint8_t scancode)
						{reinterpret_cast<Callback*>(cb_obj)->keyDown(source,scancode);};
					cursor_x=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorX(source,index,keymask);};
					cursor_y=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorY(source,index,keymask);};
					}

				void (*mouse_move)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*mouse_down)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*mouse_up)(void* cb_obj,XYPlot& source,Point x,keymask_t keymask);
				void (*key_down)(void* cb_obj,XYPlot& source,uint8_t scancode);
				void (*key_up)(void* cb_obj,XYPlot& source,uint8_t scancode);
				void (*cursor_x)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				void (*cursor_y)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				};

			XYPlot& callback(const Vtable& vt,void* cb_obj);
		};
	}

#endif // ANJA_XYPLOT_HPP
