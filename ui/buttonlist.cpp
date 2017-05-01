//@	{"targets":[{"name":"buttonlist.o","type":"object"}]}

#include "buttonlist.hpp"

using namespace Anja;

ButtonListImpl& ButtonListImpl::append(const char* const* labels)
	{
	while(*labels!=nullptr)
		{
		append(*labels);
		++labels;
		}
	return *this;
	}
