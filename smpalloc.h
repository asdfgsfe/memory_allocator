#ifndef __SMPALLOC_H__
#define __SMPALLOC_H__

#define CACHE_LINE_SIZE 128
#define GLOBAL_POOL_SIZE 40
#define TARGET_POOL_SIZE 3

struct memblock
{
	char* bytes[CHAR_LINE_SIZE];
} memblocks[GLOBAL_POOL_SIZE];


struct globalmempool
{
	spinlock_t lock;
	int cur;
	struct memblock* pool[GLOBAL_POOL_SIZE];
} globalmem;


struct perthreadmempool
{
	int cur;
	struct memblock* pool[2 * GET_POOL_SIZE];
};

DEFINE_PER_THREAD(struct perthreadmempool, perthreadmeme);

struct memblock* memblock_alloc(void);
void memblock_free(struct memblock* mem);
void init_pools(void);

#endif //__SMPALLOC_H__
