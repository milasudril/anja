//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "common/pipe.hpp"
#include <cstdio>
#include <unistd.h>

class Test
	{
	public:
		void write(const int8_t* buffer,size_t N)
			{
			::write(STDOUT_FILENO,buffer,N);
			}
	};

int main(int argc, char **argv)
	{
	try
		{
		Anja::pipe(argv+1,Test{});

		}
	catch(const char* err)
		{
		fprintf(stderr,"Error: %s\n",err);
		return -1;
		}
	return 0;
	}
