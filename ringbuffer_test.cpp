#ifdef __WAND__
target[name[ringbuffer_test] type[application] platform[;GNU/Linux] dependency[pthread;external]]
#endif

#include "ringbuffer.h"
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <random>

void sleep_short(int mus)
	{
	timespec t;
	t.tv_sec=0;
	t.tv_nsec=1000*mus;
	timespec t0;
	nanosleep(&t,&t0);
	}

void* other_thread(void* data)
	{
//	std::mt19937 randgen;
//	randgen.discard(2*65536);
//	std::uniform_int_distribution<int> U(500,4000);

	auto x=reinterpret_cast<RingBuffer<int>*>(data);
	while(1)
		{
		while(!x->empty())
			{
			printf("%d ",x->pop_front());
			}
		sleep_short(1000);
		}
	}

int main()
	{
	RingBuffer<int> test(16);
	std::mt19937 randgen;
	randgen.discard(65536);
	std::uniform_int_distribution<int> U(500,4000);

	pthread_t thread;

	pthread_create(&thread,NULL,other_thread,&test);

	int x=0;
	while(x!=100000)
		{
		if(!test.full())
			{
			test.push_back(x);
			++x;
			}
		sleep_short(1000);
		}
	}
