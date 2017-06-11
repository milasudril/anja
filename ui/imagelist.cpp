//@	{"targets":[{"name":"imagelist.o","type":"object"}]}

#include "imagelist.hpp"

using namespace Anja;

void ImageList::callbacks_assign()
	{
	int k=0;
	std::for_each(m_images.begin(),m_images.end(),[this,&k](ImageView& btn)
		{
		btn.callback(*this,k);
		++k;
		});
	}
