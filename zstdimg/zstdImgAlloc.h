#ifndef _ZSTD_IMG_ALLOC_H_
#define _ZSTD_IMG_ALLOC_H_

#include "zstdImgCompiler.h"
#include <stdlib.h>

XE_EXPORT_C_SHARP_API(void) zInitAllocFunction(void* (*def_malloc)(size_t), void* (*def_relloc)(void*, size_t), void (*def_free)(void*));

void* zmalloc(size_t size);
void* zrealloc(void* ptr, size_t size);
void zfree(void* ptr);

#endif // _ZSTD_IMG_ALLOC_H_