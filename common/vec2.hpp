//@{"targets":[{"name":"vec2.hpp","type":"include"}]}

#ifndef ANJA_VEC2_HPP
#define ANJA_VEC2_HPP

#include "vectortype.hpp"

namespace Anja
	{
	class Vec2
		{
		public:
			Vec2():m_data{0,0}
				{}

			explicit Vec2(double x,double y) noexcept:m_data{x,y}
				{}

			double x() const noexcept
				{return m_data[0];}

			double y() const noexcept
				{return m_data[1];}

			double& x() noexcept
				{return m_data[0];}

			double& y() noexcept
				{return m_data[1];}

			Vec2& operator-=(Vec2 p) noexcept
				{
				m_data-=p.m_data;
				return *this;
				}

			Vec2& operator+=(Vec2 p) noexcept
				{
				m_data+=p.m_data;
				return *this;
				}

			Vec2& componentsDiv(Vec2 p) noexcept
				{
				m_data/=p.m_data;
				return *this;
				}


			Vec2& componentsMul(Vec2 p) noexcept
				{
				m_data*=p.m_data;
				return *this;
				}

			Vec2& componentsMax(Vec2 p) noexcept
				{
				m_data=m_data>p.m_data?m_data:p.m_data;
				return *this;
				}

			Vec2& componentsMin(Vec2 p) noexcept
				{
				m_data=m_data<p.m_data?m_data:p.m_data;
				return *this;
				}

			Vec2& operator*=(double c) noexcept
				{
				m_data*=c;
				return *this;
				}


		private:
			vec2_t<double> m_data;
		};

	inline Vec2 operator-(Vec2 a,Vec2 b)
		{return a-=b;}

	inline Vec2 operator+(Vec2 a,Vec2 b)
		{return a+=b;}

	inline Vec2 componentsDiv(Vec2 a,Vec2 b)
		{return a.componentsDiv(b);}

	inline Vec2 componentsMul(Vec2 a,Vec2 b)
		{return a.componentsMul(b);}

	inline Vec2 componentsMax(Vec2 a,Vec2 b)
		{return a.componentsMax(b);}

	inline Vec2 componentsMin(Vec2 a,Vec2 b)
		{return a.componentsMin(b);}

	inline double dot(Vec2 a,Vec2 b)
		{return a.x()*b.x() + b.y()*b.y();}

	inline Vec2 operator*(double x,Vec2 a)
		{return a*=x;}
	}

#endif
