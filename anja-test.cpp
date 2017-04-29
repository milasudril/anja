//@	{"targets":[{"name":"anja-test","type":"application"}]}

#include "sessiondata/session.hpp"

int main(int argc, char **argv)
	{
	try
		{
		Anja::Session s("testbank/testbank.txt");
		s.save("__targets_dbg/output.txt");

		}
	catch(const char* err)
		{
		fprintf(stderr,"Error: %s\n",err);
		return -1;
		}
	return 0;
	}
