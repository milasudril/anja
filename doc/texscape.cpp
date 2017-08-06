//@	{"targets":[{"name":"texscape","type":"application"}]}

#include <cstdio>

int main()
	{
	enum class State:int{NORMAL,ESCAPE,DASH};

	auto state=State::NORMAL;

	while(true)
		{
		auto ch_in=getchar();
		if(ch_in==EOF)
			{return 0;}

		switch(state)
			{
			case State::NORMAL:
				switch(ch_in)
					{
					case 127:
						state=State::ESCAPE;
						break;
					case '\t':
						printf("    ");
						break;
					default:
						putchar(ch_in);
					}
				break;

			case State::ESCAPE:
				switch(ch_in)
					{
					case 127:
						state=State::NORMAL;
						break;

					case '-':
						putchar(ch_in);
						state=State::DASH;
						break;

					case '\\':
					case '#':
					case '_':
					case '~':
					case '&':
					case '%':
						putchar('\\');
						putchar(ch_in);
						break;

					case '\t':
						printf("    ");
						break;

					default:
						putchar(ch_in);
					}
				break;

			case State::DASH:
				switch(ch_in)
					{
					case 127:
						state=State::NORMAL;
						break;

					case '-':
						printf("{}-");
						state=State::ESCAPE;
						break;

					case '\\':
					case '#':
					case '_':
					case '~':
						putchar('\\');
						putchar(ch_in);
						state=State::ESCAPE;
						break;

					case '\t':
						printf("    ");
						break;

					default:
						state=State::ESCAPE;
						putchar(ch_in);
					}
				break;
			}
		}
	return 0;
	}
