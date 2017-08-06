//@	{"targets":[{"name":"texscape","type":"application"}]}

#include <cstdio>

int main()
	{
	bool run_esc=0;
	while(true)
		{
		auto ch_in=getchar();
		if(ch_in==EOF)
			{return 0;}

		if(run_esc)
			{
			switch(ch_in)
				{
				case 127:
					run_esc=0;
					break;

				case '#':
				case '_':
				case '-':
				case '\\':
				case '~':
					putchar('\\');
					putchar(ch_in);
					break;

				default:
					putchar(ch_in);
				}
			}
		else
			{
			switch(ch_in)
				{
				case 127:
					run_esc=1;
					break;
				default:
					putchar(ch_in);
				}
			}
		}
	return 0;
	}
