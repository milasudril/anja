//@	{
//@	"targets":
//@		[{"name":"anja","type":"application","include_targets":["projectinfo.hpp"]}]
//@	}

#include "ui/application.hpp"
#include <cstdio>
#include <time.h>

extern "C"
	{
	void __cyg_profile_func_enter(void *this_fn,void *call_site)
		{printf("%p\n",this_fn);}
	}

int main(int argc, char **argv)
	{
	try
		{
		Anja::Application anja;
		anja.dark(1);

		anja.layoutInspect([](int k,const Anja::Rectangle& rect)
			{
			printf("%d %.15g %.15g %.15g %.15g\n",k
				,rect.min.x(),rect.min.y()
				,rect.max.x(),rect.max.y());
			});
		anja.run();
		}
	catch(const Anja::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}
	return 0;
	}
