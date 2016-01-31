#ifdef __WAND__
target[name[titleview.h] type[include]]
#endif

#ifndef TITLEVIEW_H
#define TITLEVIEW_H

class TitleView
	{
	public:
		virtual void titleSet(const char* title_new)=0;
	};

#endif
