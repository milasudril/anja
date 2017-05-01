//@	{"targets":[{"name":"optionlist.o","type":"object"}]}

#include "optionlist.hpp"

using namespace Anja;

OptionListImpl& OptionListImpl::selected(uint64_t mask)
	{
	auto k=std::min(size(),8*sizeof(mask));
	while(k)
		{
		--k;
		auto v=1<<k;
		m_options[k].state(v&mask);
		}
	return *this;
	}


OptionListImpl& OptionListImpl::append(const char* const* labels)
	{
	while(*labels!=nullptr)
		{
		append(*labels);
		++labels;
		}
	return *this;
	}
