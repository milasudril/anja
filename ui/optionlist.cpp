//@	{"targets":[{"name":"optionlist.o","type":"object"}]}

#include "optionlist.hpp"

using namespace Anja;

OptionList& OptionList::selected(uint64_t mask)
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


OptionList& OptionList::append(const char* const* labels)
	{
	while(*labels!=nullptr)
		{
		append(*labels);
		++labels;
		}
	return *this;
	}

void OptionList::callbacks_assign()
	{
	int k=0;
	std::for_each(m_options.begin(),m_options.end(),[this,&k](Checkbox& btn)
		{
		btn.callback(*this,k);
		++k;
		});
	}
