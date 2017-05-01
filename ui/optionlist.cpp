//@	{"targets":[{"name":"optionlist.o","type":"object"}]}

#include "optionlist.hpp"

using namespace Anja;

OptionListImpl& OptionListImpl::append(const char* const* labels)
	{
	while(*labels!=nullptr)
		{
		append(*labels);
		++labels;
		}
	return *this;
	}
