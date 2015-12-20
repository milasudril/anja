#ifdef __WAND__
target[name[curve.h] type[include]]
dependency[curve.o]
#endif

#ifndef CURVE_H
#define CURVE_H

#include <cstddef>

class ColorRGBA;

class Curve
	{
	public:
		struct alignas(16) Point
			{
			double x;
			double y;
			};

		struct alignas(32) Domain
			{
			Point min;
			Point max;
			};

		Curve(const Point* points,size_t n_points):
			r_points(points),m_n_points(n_points),m_hue(0)
			{domainUpdate();}

		Curve(const Point* points,size_t n_points,float hue):
			r_points(points),m_n_points(n_points),m_hue(hue)
			{domainUpdate();}

		Curve(const Point* points,size_t n_points,const ColorRGBA& color)
			:r_points(points),m_n_points(n_points)
			{
			domainUpdate();
			colorSet(color);
			}

		size_t nPointsGet() const
			{return m_n_points;}

		const Point* pointsGet() const
			{return r_points;}

		const Domain& domainGet() const
			{return m_domain;}

		float colorGet() const
			{return m_hue;}

		void colorSet(const ColorRGBA& color);

	private:
		void domainUpdate();

		const Point* r_points;
		size_t m_n_points;
		float m_hue;
		Domain m_domain;
	};

Curve::Domain domainAlign(const Curve::Domain& d,double delta_x,double delta_y);
Curve::Domain domainAlignX(const Curve::Domain& d,double delta_x);
Curve::Domain domainAlignY(const Curve::Domain& d,double delta_y);
bool belongsTo(const Curve::Point& point, const Curve::Domain& d);
Curve::Point mapToDomainInvertY(const Curve::Point& point
	,const Curve::Domain& source,const Curve::Domain& target);
double mapToDomainX(double x,const Curve::Domain& source
	,const Curve::Domain& target);
double mapToDomainY(double y,const Curve::Domain& source
	,const Curve::Domain& target);

#endif
