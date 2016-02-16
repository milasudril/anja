#ifdef __WAND__
target[name[pathutils-test] type[application]]
#endif

#include "pathutils.h"
#include <cstdio>

int main()
	{
	auto path=makeRelativeTo("/foo/other/path","/foo/bar/some/path");
	printf("1 %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one/foo","/foo/bar/one/foo/onemore");
	printf("2 %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one","/foo/bar/two");
	printf("3 %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one/more/directory","/foo/bar");
	printf("4 %s\n\n",path.begin());

	path=makeRelativeTo("/home/torbjorr/laptop_r4/Dokument/anja/testbank/alien.wav"
		,"/home/torbjorr/Skrivbord");
	printf("5 %s\n\n",path.begin());
	return 0;
	}
