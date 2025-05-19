#include "zstdImgInit.h"
#include <string.h>

#include "zstdImgAlloc.h"
#include "zstdImgChildRead.h"

Zimg zLoadImgData(uint64_t x, uint64_t y, uint32_t channel, uint32_t channel_byte_size, const byte_t* data)
{
	Zimg output_img = { 0 };

	uint64_t size = x * y * channel * channel_byte_size;

	output_img.channel = channel;
	output_img.channel_byte_size = channel_byte_size;
	output_img.x = x;
	output_img.y = y;

	output_img.data = (byte_t*)zmalloc(size);

	memcpy(output_img.data, data, size);
	return output_img;
}

void zReleaseImgData(Zimg img)
{
	zfree(img.data);
}

void zReleaseZimgMemoryFile(ZimgMemoryFile mem_file)
{
	zfree(mem_file.data);
}
