#ifndef _ZSTD_IMG_READ_H_
#define _ZSTD_IMG_READ_H_

#include "zstdImgFormat.h"
#include "zstdImgCompiler.h"

#include <stdio.h>

namespace zimg
{
	class ChildImageReader
	{
	private:

		const MainImgReadContext* pcontest;

	public:
		ChildImageReader(const MainImgReadContext* pthis_contest) :pcontest(pthis_contest) {}

		ZimgReturn GetChildImageLine(byte_t* data_out, int32_t line_index, int32_t number);

		ZimgReturn GetChildImageAllData(byte_t* data);
	};

	class MainImgReadContext	// image file context
	{
	public:

		explicit MainImgReadContext(const byte_t* mem_data, const uint64_t mem_data_size) 
			:file_data(mem_data), data_size(mem_data_size) 
		{
			img_info = { 0 };
		}

		ZimgReturn InitMainContext();

		ImgInfo GetChildInfo();

		ZimgReturn GetChildrenImage(uint64_t child_x_index, uint64_t child_y_index, ChildImageReader& child_img);

	private:

		ImgInfo img_info;

		const byte_t* file_data;
		const uint64_t data_size;
	};
}

#endif // _ZSTD_IMG_READ_H_