#include "zstd.h"

#include "zstdImgRead.h"
#include "zstdImgAlloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

namespace zimg
{
	int64_t ChildImageReader::DecompressLineData(const byte_t* src, byte_t* dst)
	{
		// 获取压缩行大小
		const ZimgCompressLine* compressed_line_data = reinterpret_cast<const ZimgCompressLine*>(src);
		int64_t out_size = static_cast<int64_t>(compressed_line_data->compress_data_size);
		// 解压
		auto decompress_data_size = ZSTD_decompress(dst, GetImgLineAllocSize(), compressed_line_data->compress_data, compressed_line_data->compress_data_size);
		uint32_t code = ZSTD_isError(decompress_data_size);
		if (code)
		{
			printf("<LIB: ZSTD> ZSTD_decompress error: %s", ZSTD_getErrorName(decompress_data_size));
			return static_cast<int64_t>(ZimgReturn::Z_DECOMPRESS_ERROR);
		}
		return static_cast<int64_t>(ZimgReturn::Z_OK) + out_size;
	}

	ChildImageReader::LineNumber ChildImageReader::GetChildImageLine(byte_t* data_out, uint64_t line_index, ChildImageReader::LineNumber number)
	{
		uint64_t read_line_number = number;
		const byte_t* cur_compressed_data = main_context->file_data + child_info.start;
		int64_t jump_mem = 0;
		// 计算偏移
		if ((line_index + number) > GetChildImgYNumber())
		{
			puts("<FUNCTION ChildImageReader::GetChildImageLine> Warning Out of line");
			read_line_number = GetChildImgYNumber() - line_index;
		}

		for (uint64_t i = 0; i < read_line_number; i++)
		{
			jump_mem = DecompressLineData(cur_compressed_data, data_out);
			if (jump_mem < 0)
			{
				return jump_mem; // 错误
			}
			cur_compressed_data += jump_mem;
		}
		return read_line_number;
	}

	ChildImageReader::LineNumber ChildImageReader::GetChildImageAllData(byte_t* out_data)
	{
		return GetChildImageLine(out_data, 0, GetChildImgYNumber());
	}	

	ZimgReturn MainImgReadContext::InitMainContext()
	{
		// 图片头部信息
		ZimgFileHeader img_header;
		// 内存偏移
		const byte_t* cur_data_ptr = file_data;

		// 验证头
		memcpy(&img_header, cur_data_ptr, sizeof(ZimgFileHeader));
		if (img_header.file_header_text != HeaderText::FILE_HEADER_TEXT)
		{
			puts("<FUNCTION: MainImgReadContext::InitMainContext> File header is broken or not zimg file");
			return ZimgReturn::Z_FILE_BROKEN;
		}
		img_info = img_header.info;
		cur_data_ptr += sizeof(ZimgFileHeader);

		for (uint64_t i = 0; i < img_info.child_img_number; i++)
		{
			ZimgChildHeader child_img_header;
			// 读取子图片信息
			memcpy(&child_img_header, cur_data_ptr, sizeof(ZimgChildHeader));
			// 读取标记字符串
			const char* text = reinterpret_cast<const char*>(cur_data_ptr) + sizeof(ChildImgInfo);
			child_img_map->insert(std::pair<std::string, ChildImgInfo>(std::string(text), child_img_header.info));
			cur_data_ptr += child_img_header.header_ooffset;
		}
		
		return ZimgReturn::Z_OK;
	}

	ZimgReturn MainImgReadContext::GetChildrenImage(std::string& index, ChildImageReader& child_img_out)
	{
		auto found = child_img_map->find(index);
		if (found == child_img_map->end())
		{
			return ZimgReturn::Z_NOT_FIND;
		}
		else
		{
			child_img_out = ChildImageReader(this, found->second);
		}
		return ZimgReturn::Z_OK;
	}
}
