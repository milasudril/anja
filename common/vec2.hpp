//@{"targets":[{"name":"vec2.hpp","type":"include"}]}

#ifndef ANJA_VEC2_HPP
#define ANJA_VEC2_HPP

#include "vectortype.hpp"

namespace Anja
	{
	template<class T>
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

			Vec2& operator/=(double c) noexcept
				{
				m_data/=c;
				return *this;
				}


		private:
			vec2_t<T> m_data;
		};

	template<class T>
	inline Vec2<T> operator-(Vec2<T> a,Vec2<T> b)
		{return a-=b;}

	template<class T>
	inline Vec2<T> operator+(Vec2<T> a,Vec2<T> b)
		{return a+=b;}

	template<class T>
	inline Vec2<T> componentsDiv(Vec2<T> a,Vec2<T> b)
		{return a.componentsDiv(b);}

	template<class T>
	inline Vec2<T> componentsMul(Vec2<T> a,Vec2<T> b)
		{return a.componentsMul(b);}

	template<class T>
	inline Vec2<T> componentsMax(Vec2<T> a,Vec2<T> b)
		{return a.componentsMax(b);}

	template<class T>
	inline Vec2<T> componentsMin(Vec2<T> a,Vec2<T> b)
		{return a.componentsMin(b);}

	template<class T>
	inline double dot(Vec2<T> a,Vec2<T> b)
		{return a.x()*b.x() + b.y()*b.y();}

	template<class T>
	inline Vec2<T> operator*(double x,Vec2<T> a)
		{return a*=x;}

	template<class T>
	inline Vec2<T> operator/(Vec2<T> a,double x)
		{return a/=x;}
	}

#endif
