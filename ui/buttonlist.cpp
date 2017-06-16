//@	{"targets":[{"name":"buttonlist.o","type":"object"}]}

#include "buttonlist.hpp"

using namespace Anja;

ButtonList& ButtonList::append(const char* const* labels)
	{
	while(*labels!=nullptr)
		{
		append(*labels);
		++labels;
		}
	return *this;
	}

void ButtonList::callbacks_assign()
	{
	int k=0;
	std::for_each(m_buttons.begin(),m_buttons.end(),[this,&k](Button& btn)
		{
		btn.callback(*this,k);
		++k;
		});
	}
