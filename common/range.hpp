//@	{"targets":[{"name":"range.hpp","type":"include"}]}

#ifndef ANJA_RANGE_HPP
#define ANJA_RANGE_HPP

#include <algorithm>

namespace Anja
	{
	template<class RangeA,class RangeB=RangeA>
	bool compareLexicographical(const RangeA& a,const RangeB& b)
		{return std::lexicographical_compare (a.begin(), a.end(),b.begin(), b.end());}
	}

#endif
