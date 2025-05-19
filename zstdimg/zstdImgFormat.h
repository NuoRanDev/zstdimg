#ifndef _INC_ZSTD_IMG_FORMAT_H_
#define _INC_ZSTD_IMG_FORMAT_H_

#include <cstdint>
#include <queue>

using byte_t = uint8_t;

constexpr uint64_t xeGetCompressType(const char data[8])
{
	uint64_t output = 0;
	for (int i = 7; i >= 0; i--)
	{
		output = (output << 8) + data[i];
	}
	return output;
}

constexpr auto FILE_HEADER_TEXT = xeGetCompressType("zimg\0\0\0");

enum class ZimgReturn :uint32_t
{
	Z_OK = 0,
	Z_COMPRESS_ERROR,
	Z_DECOMPRESS_ERROR,
	Z_FILE_BROKEN,
	Z_OUT_OF_MEMORY
};

struct ImgInfo
{
	uint32_t channel;
	uint32_t channel_byte_size;

	uint64_t x, y;
	uint64_t compressed_data_size;

	int32_t child_img_x, child_img_y;
	uint64_t x_child_number, y_child_number;
};

struct ZimgFileHeader //image header
{
	uint64_t file_header_text;

	ImgInfo info;
};

typedef struct image_compressed_line
{
	uint64_t compress_data_size;
	byte_t compress_data[];
} ZimgCompressLine;

#endif // _INC_ZSTD_IMG_FORMAT_H_