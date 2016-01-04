#ifdef __WAND__
target[name[benchmark] type[application]]
#endif

#include <random>
#include <ctime>

volatile float x;

int main()
	{
	std::mt19937 randgen;
	randgen.seed();
	std::uniform_real_distribution<float> U(-1,1);
	constexpr uint32_t N_0=1024*1024*1024;

	auto N=N_0;
	auto t_start=clock();
	while(N)
		{
		x=U(randgen);
		--N;
		}
	auto t_end=clock();
	auto init=t_end-t_start;
	double T=double(init)/CLOCKS_PER_SEC;
	printf("Initial rate=%.15g/sec\n",N_0/T);

	randgen.seed();
	N=N_0;
	float sum=0;
	t_start=clock();
	while(N)
		{
		sum+=U(randgen);
		--N;
		}
	t_end=clock();
	T=double(t_end-t_start-init)/CLOCKS_PER_SEC;
	printf("sum=%.7g  Rate=%.15g/sec\n",sum,N_0/T);
	return 0;
	}
