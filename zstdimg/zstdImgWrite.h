#ifndef ZSTD_IMG_WRITE
#define ZSTD_IMG_WRITE

#include "zstdImgFormat.h"
#include "zstdImgCompiler.h"

#include <stdio.h>

XE_EXPORT_C_SHARP_API(int) zWriteImgToMemory(const Zimg img, ZimgMemoryFile* mem_data);

XE_EXPORT_C_SHARP_API(int) zWriteImgToCFile(const Zimg img, FILE* fs);

#endif // ZSTD_IMG_WRITE