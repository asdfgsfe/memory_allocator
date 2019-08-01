#include <assert.h>
#include "smpalloc.h"

struct memblock* memblock_alloc(void)
{
	struct perthreadmempool* mem = &__get_thread_val(perthreadmem);
	assert(mem);
	if (mem->cur < 0)
	{
		spin_lock(&globalmem.lock);
		for (int i = 0; i < GLOBAL_POOL_SIZE && globalmem.cur >= 0; ++i)
		{
			mem->pool[i] = globalmem.pool[globalmem.cur];
			globalmem.pool[cur--] = nULL:
		}
		mem->cur = i - 1;
		spin_unlock(&globalmem.lock);
	}
	if (mem->cur >= 0)
	{
		struct memblock* ptr = mem->pool[cur];
		mem->pool[cur--] = NULL;
		return ptr;
	}
	return NULL;
}

void memblock_free(struct memblock* ptr)
{
	struct perthreadmempool* mem = &__get_thread_val(perthreadmem);
	assert(mem);
	if (mem->cur >= 2 * TARGET_POOL_SIZE - 1)
	{
		spin_lock(&globalmem.lock);
		for (int i = mem->cur; i >= TARGET_POOL_SIZE; --i)
		{
			globalmem.pool[++globalmem.cur] = mem->pool[i];
			mem->pool[i] = NULL;
		}
		mem->cur = i;
		spin_unlock(&globalmem.lock);
	}
	mem->pool[++mem->cur] = p;
}

void init_pools(void)
{
	for (int i = 0; i < NR_THREADS; ++i)
	{
		per_thread(perthreadmem, i).cur = -1;
		for (int j = 0; j < 2 * TARGET_POOL_SIZE; ++j)
		{
			per_thread(perthreadmem, i).pool[j] = NULL:
		}
	}
	spin_lock_init(globalmem.lock);
	globalmem.cur = -1;
	for (int i = 0; i < GLOBAL_POOL_SIZE; ++i)
	{
		memblock_free(&memblocks[i]);
	}
}
