#ifdef __WAND__
target[name[color.o] type[object]]
#endif

#include "color.h"
#include <cmath>
#include <algorithm>
#include <limits>

/**Helper type
*/
struct alignas(16) ColorHMMA
	{
	float hue;
	float min;
	float max;
	float alpha;
	};

ColorRGBA::ColorRGBA(const ColorHSLA& hsla)
	{
	auto h=hsla.hue;
	auto s=hsla.saturation;
	auto l=hsla.lightness;
	auto a=hsla.alpha;

	auto c=(1-std::abs(2.0f*l - 1.0f))*s;
	auto x=c*(1.0f-std::abs( std::fmod(6.0f*h,2.0f) - 1.0f ));

	ColorRGBA ret;
	if(h < 1.0f/6.0f)
		{ret={c,x,0.0f,a};}
	else
	if(h < 2.0f/6.0f)
		{ret={x,c,0.0f,a};}
	else
	if(h < 3.f/6.0f)
		{ret={0.0f,c,x,a};}
	else
	if(h < 4.0f/6.0f)
		{ret={0.0f,x,c,a};}
	else
	if(h < 5.0f/6.0f)
		{ret={x,0.0f,c,a};}
	else
		{ret={c,0.0f,x,a};}

	auto m=l-c/2;

	ret.red+=m;
	ret.green+=m;
	ret.blue+=m;
	*this=ret;
	}

ColorRGBA::ColorRGBA(const ColorHSVA& hsva)
	{
	auto h=hsva.hue;
	auto s=hsva.saturation;
	auto v=hsva.value;
	auto a=hsva.alpha;

	auto c=v*s;

	auto x=c*(1.0f-std::abs( std::fmod(6.0f*h,2.0f) - 1.0f ));
	ColorRGBA ret;
	if(h < 1.0f/6.0f)
		{ret={c,x,0.0f,a};}
	else
	if(h < 2.0f/6.0f)
		{ret={x,c,0.0f,a};}
	else
	if(h < 3.f/6.0f)
		{ret={0.0f,c,x,a};}
	else
	if(h < 4.0f/6.0f)
		{ret={0.0f,x,c,a};}
	else
	if(h < 5.0f/6.0f)
		{ret={x,0.0f,c,a};}
	else
		{ret={c,0.0f,x,a};}

	auto m=v-c;

	ret.red+=m;
	ret.green+=m;
	ret.blue+=m;
	*this=ret;
	}

ColorHMMA hmma(const ColorRGBA& rgba)
	{
	auto r=rgba.red;
	auto g=rgba.green;
	auto b=rgba.blue;
	auto a=rgba.alpha;


	auto M=std::max(r,std::max(g,b));
	auto m=std::min(r,std::min(g,b));

	ColorHMMA ret;
	auto chroma=M-m;

	if(chroma==0.0f)
		{ret.hue=0.0f;}
	else
	if(M==r)
		{ret.hue=(g-b)/chroma;}
	else
	if(M==g)
		{ret.hue=(b-r)/chroma + 2.0f;}
	else
		{ret.hue=(r-g)/chroma + 4.0f;}

	if(ret.hue<0)
		{ret.hue+=6.0f;}

	ret.hue/=6.0f;
	ret.min=m;
	ret.max=M;
	ret.alpha=a;

	return ret;
	}

ColorHSLA::ColorHSLA(const ColorRGBA& rgba)
	{
	auto x=hmma(rgba);
	auto chroma=x.max - x.min;
	auto lightness=0.5f*(x.max+x.min);
	auto saturation=chroma==0.0f? 0.0f:chroma/(1.0f-std::abs(2.0f*lightness-1.0f));
	*this={x.hue,saturation,lightness,x.alpha};
	}

ColorHSLA ColorHSLA::fromHueSaturationLuma(float hue,float saturation,float luma_709)
	{
	auto lightness_min=0.0f;
	auto lightness_max=1.0f;
	auto lightness_current=0.5f*(lightness_min+lightness_max);
	ColorHSLA color_test{hue,saturation,lightness_current,1.0f};
	while(lightness_max - lightness_min > std::numeric_limits<float>::epsilon())
		{
		color_test.lightness=lightness_current;
		auto result=luma709(color_test);
		if(result < luma_709)
			{
			lightness_min=lightness_current;
			lightness_current=0.5f*(lightness_current + lightness_max);
			}
		else
		if(result > luma_709)
			{
			lightness_max=lightness_current;
			lightness_current=0.5f*(lightness_current + lightness_min);
			}
		else
			{return color_test;}
		}
	return color_test;
	}

ColorHSVA::ColorHSVA(const ColorRGBA& rgba)
	{
	auto x=hmma(rgba);
	auto chroma=x.max - x.min;
	auto value=x.max;
	auto saturation=chroma==0.0f? 0.0f : chroma/value;

	*this={x.hue,saturation,value,x.alpha};
	}
