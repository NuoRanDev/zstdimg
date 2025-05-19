#include "zstd.h"

#include "zstdImgRead.h"
#include "zstdImgAlloc.h"
#include "zstdImgInit.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace zimg
{
	ZimgReturn ChildImageReader::GetChildImageLine(byte_t* data_in, int32_t line_index, int32_t number)
	{
		return ZimgReturn::Z_OK;
	}

	ZimgReturn ChildImageReader::GetChildImageAllData(byte_t* data)
	{
		ZimgFileHeader f_header = { 0 };

		const byte_t* cur_data_ptr = data;

		byte_t* cur_img_data_ptr = NULL;

		uint64_t compressed_data_size = 0;
		uint64_t line_size = 0;				// compress unit

		cur_data_ptr += sizeof(ZimgFileHeader);

		memcpy(&f_header, cur_data_ptr, sizeof(ZimgFileHeader));
		cur_data_ptr += sizeof(ZimgFileHeader);

		pimg->channel = f_header.channel;
		pimg->channel_byte_size = f_header.channel_byte_size;
		pimg->x = f_header.x;
		pimg->y = f_header.y;
		pimg->data = (byte_t*)zmalloc(f_header.x * f_header.y * f_header.channel_byte_size);

		line_size = pimg->channel * pimg->channel_byte_size * pimg->x;

		cur_img_data_ptr = pimg->data;

		for (uint64_t i = 0; i < pimg->y; i++)
		{
			memcpy(&compressed_data_size, cur_data_ptr, sizeof(uint64_t));
			cur_data_ptr += sizeof(uint64_t);

			uint64_t decompress_frame_size = ZSTD_getFrameContentSize(cur_data_ptr, compressed_data_size);
			uint64_t decompress_size = ZSTD_decompress(cur_img_data_ptr, decompress_frame_size, cur_data_ptr, compressed_data_size);

			cur_data_ptr += compressed_data_size;
			cur_img_data_ptr += line_size;

			if (decompress_size != line_size)
			{
				printf("<LIB: ZSTD> Decmpressed file is broken | source size: %llu ,decommpress: %llu", line_size, decompress_size);
				zfree(pimg->data);
				return ZimgReturn::Z_FILE_BROKEN;
			}
			uint32_t code = ZSTD_isError(decompress_size);
			if (code)
			{
				printf("<LIB: ZSTD> CompressZSTD error: %s", ZSTD_getErrorName(decompress_size));
				zfree(pimg->data);
				return ZimgReturn::Z_DECOMPRESS_ERROR;
			}
		}
		return ZimgReturn::Z_OK;
	}

	ZimgReturn MainImgReadContext::InitMainContext()
	{
		ZimgFileHeader img_header;
		memcpy(&img_header, file_data, sizeof(ZimgFileHeader));
		if (img_header.file_header_text != FILE_HEADER_TEXT)
		{
			puts("<FUNCTION: MainImgReadContext::InitMainContext> File header is broken or not zimg file");
			return ZimgReturn::Z_FILE_BROKEN;
		}
		img_info = img_header.info;
		return ZimgReturn::Z_OK;
	}

	ImgInfo MainImgReadContext::GetChildInfo()
	{
		return img_info;
	}

	ZimgReturn MainImgReadContext::GetChildrenImage(uint64_t child_x_index, uint64_t child_y_index, ChildImageReader& child_img)
	{
		uint64_t index = child_x_index + img_info.x_child_number * child_y_index;
		const byte_t* cur_data_ptr = file_data;

		if (index > img_info.x_child_number * img_info.y_child_number || index < 0)
		{
			puts("<FUNCTION: MainImgReadContext::GetChildrenImage> Out of child number");
			return ZimgReturn::Z_OUT_OF_MEMORY;
		}

		file_data += sizeof(ZimgFileHeader);

		for (uint64_t i = 0; i < ind; i++)
		{

		}

		return ZimgReturn::Z_OK;
	}
}
