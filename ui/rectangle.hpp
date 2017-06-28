//@	{"targets":[{"name":"rectangle","type":"include"}]}

#ifndef ANJA_RECTANGLE_HPP
#define ANJA_RECTANGLE_HPP

#include "../common/vec2.hpp"

namespace Anja
	{
	struct Rectangle
		{
		Vec2<double> min;
		Vec2<double> max;
		};
	}

#endif // ANJA_RECTANGLE_HPP
