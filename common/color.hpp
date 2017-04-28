//@	{
//@	 "targets":[{"name":"color.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"color.o","rel":"implementation"}]
//@	}

#ifndef ANJA_COLOR_HPP
#define ANJA_COLOR_HPP

#include <cstdint>
#include <algorithm>

namespace Anja
	{
	struct ColorHSLA;
	struct ColorHSVA;
	struct ColorRGBA;

	constexpr float clamp(float x,float a,float b)
		{
		return x<a?
			a:(b<x?
				b:x);
		}

	struct alignas(16) ColorRGBA
		{
		constexpr ColorRGBA():red(0),green(0),blue(0),alpha(1)
			{}

		explicit constexpr ColorRGBA(float r,float g,float b,float a):
			 red(clamp(r,0.0f,1.0f))
			,green(clamp(g,0.0f,1.0f))
			,blue(clamp(b,0.0f,1.0f))
			,alpha(clamp(a,0.0f,1.0f))
			{}

		ColorRGBA(const ColorHSLA& hsla);
		ColorRGBA(const ColorHSVA& hsva);

		float red;
		float green;
		float blue;
		float alpha;
		};

	constexpr ColorRGBA COLORS[]=
		{
		 ColorRGBA{1.0f,1.0f,1.0f,1.0f}    //WHITE
		,ColorRGBA{0.75f,0.75f,0.75f,1.0f} //GRAY
		,ColorRGBA{0.25f,0.25f,0.25f,1.0f} //GRAY_DARK
		,ColorRGBA{0.0f,0.0f,0.0f,1.0f}    //BLACK
		,ColorRGBA{1.0f,0.0f,0.0f,1.0f}    //RED
		,ColorRGBA{1.0f,0.5f,0.0f,1.0f}    //ORANGE
		,ColorRGBA{1.0f,1.0f,0.0f,1.0f}    //YELLOW
		,ColorRGBA{0.5f,1.0f,0.0f,1.0f}    //LIME
		,ColorRGBA{0.0f,1.0f,0.0f,1.0f}    //GREEN
		,ColorRGBA{0.0f,1.0f,0.5f,1.0f}    //TURQUOISE
		,ColorRGBA{0.0f,1.0f,1.0f,1.0f}    //CYAN
		,ColorRGBA{0.0f,0.5f,1.0f,1.0f}    //SKY
		,ColorRGBA{0.0f,0.0f,1.0f,1.0f}    //BLUE
		,ColorRGBA{0.5f,0.0f,1.0f,1.0f}    //VIOLETT
		,ColorRGBA{1.0f,0.0f,1.0f,1.0f}    //MAGENTA
		,ColorRGBA{1.0f,0.0f,0.5f,1.0f}    //PINK
		};

	namespace ColorID
		{
		enum
			{
			 WHITE,GRAY,GRAY_DARK,BLACK
			,RED,ORANGE,YELLOW,LIME
			,GREEN,TURQOISE,CYAN,SKY
			,BLUE,VIOLETT,MAGNTA,PINK
			,COLOR_END
			};
		}

	constexpr float luma709(const ColorRGBA& rgba)
		{
		return 0.2126f*rgba.red + 0.7152f*rgba.green + 0.0722f*rgba.blue;
		}

	constexpr bool isDark(const ColorRGBA& rgba)
		{
		return luma709(rgba)<0.5f;
		}

	constexpr bool isLight(const ColorRGBA& rgba)
		{return !isDark(rgba);}

	struct alignas(16) ColorHSLA
		{
		constexpr ColorHSLA(float h,float s,float l,float a):
			hue(h),saturation(s),lightness(l),alpha(a)
			{}

		ColorHSLA(const ColorRGBA& rgba);
		static ColorHSLA fromHueSaturationLuma(float hue,float saturation,float luma_709);

		static ColorHSLA fromHueAndLuma(float hue,float luma_709)
			{return fromHueSaturationLuma(hue,1.0f,luma_709);}

		float hue;
		float saturation;
		float lightness;
		float alpha;
		};

	struct alignas(16) ColorHSVA
		{
		constexpr ColorHSVA(float h,float s,float v,float a):
			hue(h),saturation(s),value(v),alpha(a)
			{}

		ColorHSVA(const ColorRGBA& rgba);

		float hue;
		float saturation;
		float value;
		float alpha;
		};
	}
#endif
