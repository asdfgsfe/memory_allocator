# memory_allocator
### Kernel-level memory allocator
Thread_local cache and quick slow path algorithm.  
#### The memory allocator has the following layers.  
1.Kernel layer e.g glibc  
2.malloc/tcmalloc layel  
3.c++ new/delete layel  
4.stl allocator  
5.memory pool, Application of producer consumer model. e.g muduo buffer zreoMQ yqueuq_t   
