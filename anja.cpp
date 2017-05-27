//@	{"targets":[{"name":"anja","type":"application"}]}

#include "ui/application.hpp"
#include <maike/targetinclude.hpp>
#include MAIKE_TARGET(projectinfo.hpp)
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
		anja.sessionLoad("testbank/testbank.txt");
		anja.run();
		}
	catch(const Anja::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}
	return 0;
	}
