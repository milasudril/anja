#ifdef __WAND__
target[name[range.h] type[include]]
#endif

#ifndef RANGE_H
#define RANGE_H

#include <algorithm>

template<class RangeA,class RangeB=RangeA>
bool compareLexicographical(const RangeA& a,const RangeB& b)
	{return std::lexicographical_compare (a.begin(), a.end(),b.begin(), b.end());}

#endif
