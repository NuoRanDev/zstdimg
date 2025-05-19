#include "zstdImgAlloc.h"

static void* (*z_malloc)(size_t)		= &malloc;
static void* (*z_relloc)(void*, size_t)	= &realloc;
static void (*z_free)(void*)			= &free;


void zInitAllocFunction(void* (*def_malloc)(size_t), void* (*def_relloc)(void*, size_t), void (*def_free)(void*))
{
	z_malloc = def_malloc;
	z_relloc = def_relloc;
	z_free = def_free;
}

void* zmalloc(size_t size) 
{
	return z_malloc(size); 
}
void* zrealloc(void* ptr, size_t size) 
{
	return z_relloc(ptr, size); 
}
void zfree(void* ptr) 
{
	z_free(ptr); 
}