#ifndef _ZSTD_IMG_INIT_
#define _ZSTD_IMG_INIT_

#include "zstdImgFormat.h"
#include "zstdImgCompiler.h"

XE_EXPORT_C_SHARP_API(Zimg) zLoadImgData(uint64_t x, uint64_t y, uint32_t channel, uint32_t channel_byte_size, const byte_t* data);

XE_EXPORT_C_SHARP_API(void) zReleaseImgData(Zimg img);

XE_EXPORT_C_SHARP_API(void) zReleaseZimgMemoryFile(ZimgMemoryFile mem_file);

#endif // _ZSTD_IMG_INIT_