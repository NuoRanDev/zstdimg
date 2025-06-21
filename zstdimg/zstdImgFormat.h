#ifndef _INC_ZSTD_IMG_FORMAT_H_
#define _INC_ZSTD_IMG_FORMAT_H_

#include <cstdint>

using byte_t = uint8_t;

// 此处用于合并定长字符串内存
constexpr uint64_t Str2Uint64(const char data[8])
{
	uint64_t output = 0;
	for (int i = 7; i >= 0; i--)
	{
		output = (output << 8) + data[i];
	}
	return output;
}

enum class ZimgReturn :int64_t
{
	// 运行没问题
	Z_OK = 0,
	// 压缩问题
	Z_COMPRESS_ERROR = -1,
	// 解压缩问题
	Z_DECOMPRESS_ERROR = -2,
	// 文件已破坏
	Z_FILE_BROKEN = -3,
	// 电脑爆内存
	Z_OUT_OF_MEMORY = -4,
	//
	Z_NOT_FIND = -5
};

namespace HeaderText
{
	const uint64_t FILE_HEADER_TEXT = Str2Uint64("zimg\0\0\0");
	const uint64_t BLOCK_HEADER_TEXT = Str2Uint64("zblock");
}

// 子图片信息
struct ChildImgInfo
{
	// 颜色通道
	uint32_t channel;
	
	// 颜色深度
	uint32_t channel_byte_size;
	
	// 当前压缩数据的大小
	uint64_t compressed_data_size;
	
	// 子图片占用瓦块的宽度和高度 
	int64_t child_img_x_block, child_img_y_block;

	// 子图片数据偏移
	uint64_t start, offset;
};

// 总图片信息
struct MainImgInfo
{
	// 横向/纵向 网格数量
	uint64_t x_block_number, y_block_number;
	// 网格大小
	uint64_t block_size_x, block_size_y;
	// 子图片数量
	uint64_t child_img_number;
};

// 总图片头
struct ZimgFileHeader //image header
{
	uint64_t file_header_text;

	MainImgInfo info;
};

// 子图片头
struct ZimgChildHeader
{
	// 子图片头偏移
	uint64_t header_ooffset;
	ChildImgInfo info;
};

// 瓦片图片行数据
struct ZimgCompressLine
{
	uint64_t compress_data_size;
	byte_t compress_data[];
};

#endif // _INC_ZSTD_IMG_FORMAT_H_