//@	{
//@	"targets":[{"name":"xyplot.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"xyplot.o","rel":"implementation"}]
//@	}

#ifndef ANJA_XYPLOT_HPP
#define ANJA_XYPLOT_HPP

#include "keymask.hpp"
#include "../common/vec2.hpp"
#include <utility>
#include <cmath>

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

			int id() const noexcept;

			template<class Callback,class IdType>
			XYPlot& callback(Callback& cb,IdType id) noexcept
				{return callback(Vtable(cb,id),&cb,static_cast<int>(id));}

			typedef Vec2 Point;

			template<class PointRange>
			XYPlot& curve(const PointRange& points,float hue)
				{return curve(points.begin(),points.end(),hue);}

			XYPlot& curve(const Point* begin,const Point* end,float hue);
			XYPlot& curvesRemove();

			struct Domain
				{
				Domain():min(Point{INFINITY,INFINITY}),max(Point{-INFINITY,-INFINITY})
					{}

				explicit Domain(Point m,Point M):min(m),max(M)
					{}

				Point min;
				Point max;
				};

			XYPlot& domain(const Domain& dom) noexcept;
			Domain domain() const noexcept;

			struct Cursor
				{
				double position;
				float hue;
				bool dashed;
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
					cursor_x_move=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorXMove(source,static_cast<IdType>(source.id()),index,keymask);};
					cursor_y_move=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorYMove(source,static_cast<IdType>(source.id()),index,keymask);};
					cursor_x_rightclick=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorXRightclick(source,static_cast<IdType>(source.id()),index,keymask);};
					cursor_y_rightclick=[](void* cb_obj,XYPlot& source,int index,keymask_t keymask)
						{reinterpret_cast<Callback*>(cb_obj)->cursorYRightclick(source,static_cast<IdType>(source.id()),index,keymask);};
					}

				void (*cursor_x_move)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				void (*cursor_y_move)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				void (*cursor_x_rightclick)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				void (*cursor_y_rightclick)(void* cb_obj,XYPlot& source,int index,keymask_t keymask);
				};

			XYPlot& callback(const Vtable& vt,void* cb_obj,int id) noexcept;
		};
	}

#endif // ANJA_XYPLOT_HPP
