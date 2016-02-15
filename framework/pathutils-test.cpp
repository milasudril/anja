#ifdef __WAND__
target[name[pathutils-test] type[application]]
#endif

#include "pathutils.h"
#include <cstdio>

int main()
	{
	auto path=makeRelativeTo("/foo/other/path","/foo/bar/some/path/");
	printf("1A %s\n\n",path.begin());

	path=makeRelativeTo("/foo/other/path","/foo/bar/some/path");
	printf("1B %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one/foo","/foo/bar/one/foo/onemore/");
	printf("2A %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one/foo","/foo/bar/one/foo/onemore");
	printf("2B %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one","/foo/bar/two/");
	printf("3A %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one","/foo/bar/two");
	printf("3B %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one.txt","/foo/bar/");
	printf("4A %s\n\n",path.begin());

	path=makeRelativeTo("/foo/bar/one.txt","/foo/bar");
	printf("4B %s\n\n",path.begin());
	return 0;
	}
