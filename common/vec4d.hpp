//@	{"targets":[{"name":"vec4d.hpp","type":"include"}]}

#include <xmmintrin.h>
#ifdef __AVX__
#include <immintrin.h>
#endif

#ifndef ANJA_VEC4D_HPP
#define ANJA_VEC4D_HPP

#include "vectortype.hpp"

namespace Anja
	{
#ifdef __AVX__
	struct Vec4dAVX
		{
		operator vec4_t<float>() const noexcept
			{return _mm256_cvtpd_ps(m_data);}

		Vec4dAVX& operator*=(Vec4dAVX v) noexcept
			{
			m_data*=v.m_data;
			return *this;
			}

		vec4_t<double> m_data;

		double operator[](int x) const noexcept
			{return m_data[x];}

		Vec4dAVX& reverse() noexcept
			{
		//TODO:__m256d _mm256_permute_pd (__m128d a, int imm8)
			std::swap(m_data[3],m_data[0]);
			std::swap(m_data[2],m_data[1]);
			return *this;
			}

		};

	inline Vec4dAVX min(Vec4dAVX a,Vec4dAVX b) noexcept
		{return {a.m_data<b.m_data? a.m_data : b.m_data}; }

	inline Vec4dAVX max(Vec4dAVX a,Vec4dAVX b) noexcept
		{return {a.m_data<b.m_data? b.m_data : a.m_data}; }

	inline Vec4dAVX clamp(Vec4dAVX a,Vec4dAVX b,Vec4dAVX v) noexcept
		{return max(a,min(b,v));}

	inline Vec4dAVX operator*(double c,Vec4dAVX v) noexcept
		{return Vec4dAVX{c*v.m_data};}

#endif
	struct Vec4dSSE2
		{
		vec2_t<double> x;
		vec2_t<double> y;

		operator vec4_t<float>() const noexcept
			{
			auto a=_mm_cvtpd_ps(x);
			auto b=_mm_cvtpd_ps(y);
			auto tmp=_mm_bslli_si128(reinterpret_cast<vec2_t<long long int>>(b),8);
			b=reinterpret_cast<vec4_t<float>>(tmp);
			return a + b;
			}

		Vec4dSSE2& operator*=(const Vec4dSSE2& v) noexcept
			{
			x*=v.x;
			y*=v.y;
			return *this;
			}

		double operator[](int x) const noexcept
			{return *(reinterpret_cast<const double*>(this) + x);}

		Vec4dSSE2& reverse() noexcept
			{
			std::swap(x,y);
		//TODO: __m128d _mm_shuffle_pd (__m128d a, __m128d b, int imm8);
			std::swap(x[0],x[1]);
			std::swap(y[0],x[1]);
			return *this;
			}
		};


	inline Vec4dSSE2 min(Vec4dSSE2 a,Vec4dSSE2 b) noexcept
		{
		return {a.x<b.x? a.x : b.x , a.y<b.y? a.y : b.y};
		}

	inline Vec4dSSE2 max(Vec4dSSE2 a,Vec4dSSE2 b) noexcept
		{
		return {a.x<b.x? b.x : a.x , a.y<b.y? b.y : a.y };
		}

	inline Vec4dSSE2 clamp(Vec4dSSE2 a,Vec4dSSE2 b,Vec4dSSE2 v) noexcept
		{return max(a,min(b,v));}


	inline Vec4dSSE2 operator*(double c,Vec4dSSE2 v) noexcept
		{return Vec4dSSE2{c*v.x,c*v.y};}

	}

#endif // ANJA_VEC4D_HPP
