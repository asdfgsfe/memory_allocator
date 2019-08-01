#include "smpalloc.h"

DEFINE_PER_THREAD(long, results);
DEFINE_PER_THREAD(long, failures);

#define MAX_RUN 100
int goflag;

int memblocks_available(void)
{
	int sum = globalmem.cur + 1;
	int i = 0;
	for_each_thread(i)
		sum += per_thread(perthreadmem, i).cur + 1;
	return sum;
}

void* memblocks_test(void* arg)
{
	int runlength = (intptr_t)arg;
	runlength = runlength > MAX_RUN ? MAX_RUN : runlength;
	long cnt = 0;
	long failcnt = 0;
	struct memblock* p[MAX_RUN];
	while (goflag)
	{
		for (int i = 0; i < runlength; ++i)
		{
			p[i] = memblock_alloc();
		}
		for (int i = 0; i < runlength; ++i)
		{
			if (p[i] == NULL)
			{
				++failcnt;
			}
			else
			{
				memblock_free(p[i]);
				++cnt;
			}
		}
	}
	__get_thread_val(results) = cnt;
	__get_thrad_val(failures) = failcnt;
	return NULL;
}

int main(void)
{
	smp_init();
	init_pools();
	int threadsNum = 2;
	init_per_thread(results, 0L);
	init_per_thread(failures, 0L);
	int totbefore = memblocks_available();
	int runlength = 1;
	goflag = 1;
	for (int i = 0; i < threadNums; ++i)
	{
		create_thread(memblocks_test, (void*)(intptr_t)runlength);
	}
	
	sleep(1);
	goflag = 0;
	wait_all_thread();
	long long nc = 0;
	long long nf = 0;
	for (int i = 0; i < threadsNum; ++i)
	{
		nc += per_thread(results, i);
		nf += per_thread(failures, i);
	}
	printf("a/f %ld, fail=%ld\n", nc, nf);
	if (memblocks_available() != totbefore)
	{
		printf("memblocks before=%d, after=%d\n", totbefore, memblocks_available());
	}

	return 0;
}
