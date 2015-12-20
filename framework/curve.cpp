#ifdef __WAND__
target[name[curve.o] type[object]]
#endif

#include "curve.h"
#include "color.h"
#include <cmath>

void Curve::domainUpdate()
	{
	auto ptr=r_points;
	auto N=m_n_points;
	if(N==0)
		{return;}

	Domain domain_new{*ptr,*ptr};

	--N;
	++ptr;
	while(N!=0)
		{
		domain_new.min.x = ptr->x < domain_new.min.x?
			ptr->x : domain_new.min.x;
		domain_new.min.y = ptr->y < domain_new.min.y?
			ptr->y : domain_new.min.y;

		domain_new.max.x = ptr->x > domain_new.max.x?
			ptr->x : domain_new.max.x;
		domain_new.max.y = ptr->y > domain_new.max.y?
			ptr->y : domain_new.max.y;

		++ptr;
		--N;
		}
	m_domain=domain_new;
	}

void Curve::colorSet(const ColorRGBA& color)
	{
	m_hue=ColorHSLA(color).hue;
	}

bool belongsTo(const Curve::Point& point,const Curve::Domain& domain)
	{
	if( point.x>=domain.min.x && point.x<=domain.max.x
		&& point.y>=domain.min.y && point.y<=domain.max.y)
		{return 1;}
	return 0;
	}

Curve::Domain domainAlign(const Curve::Domain& d,double delta_x,double delta_y)
	{
	auto x_min=delta_x*floor(d.min.x/delta_x);
	auto y_min=delta_y*floor(d.min.y/delta_y);
	auto x_max=delta_x*ceil(d.max.x/delta_x);
	auto y_max=delta_y*ceil(d.max.y/delta_y);
	return {{x_min,y_min},{x_max,y_max}};
	}

Curve::Domain domainAlignX(const Curve::Domain& d,double delta_x)
	{
	auto x_min=delta_x*floor(d.min.x/delta_x);
	auto y_min=d.min.y;
	auto x_max=delta_x*ceil(d.max.x/delta_x);
	auto y_max=d.max.y;
	return {{x_min,y_min},{x_max,y_max}};
	}

Curve::Domain domainAlignY(const Curve::Domain& d,double delta_y)
	{
	auto x_min=d.min.x;
	auto y_min=delta_y*floor(d.min.y/delta_y);
	auto x_max=d.max.x;
	auto y_max=delta_y*ceil(d.max.y/delta_y);
	return {{x_min,y_min},{x_max,y_max}};
	}

Curve::Point mapToDomainInvertY(const Curve::Point& p,const Curve::Domain& source
	,const Curve::Domain& target)
	{
	auto d=source;
	auto w=target;
	auto xi=(p.x-d.min.x)/(d.max.x - d.min.x);
	auto eta=(p.y-d.min.y)/(d.max.y - d.min.y);
	return
		{
		xi*w.max.x+(1-xi)*w.min.x
		,eta*w.min.y+(1-eta)*w.max.y
		};
	}

double mapToDomainX(double x,const Curve::Domain& source
	,const Curve::Domain& target)
	{
	auto d=source;
	auto w=target;
	auto xi=(x-d.min.x)/(d.max.x - d.min.x);
	return xi*w.max.x+(1-xi)*w.min.x;
	}

double mapToDomainY(double y,const Curve::Domain& source
	,const Curve::Domain& target)
	{
	auto d=source;
	auto w=target;
	auto xi=(y-d.min.y)/(d.max.y - d.min.y);
	return xi*w.max.y+(1-xi)*w.min.y;
	}
