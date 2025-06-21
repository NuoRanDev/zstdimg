#ifndef _ZSTD_IMG_READ_H_
#define _ZSTD_IMG_READ_H_

#include "zstdImgFormat.h"
#include "zstdImgCompiler.h"

#include <stdio.h>
#include <string>
#include <map>

namespace zimg
{
	class ChildImageReader
	{
		using LineNumber = uint64_t;
	private:

		const ChildImgInfo child_info;

		const MainImgReadContext* main_context;

		int64_t DecompressLineData(const byte_t* src, byte_t* dst);

	public:
		ChildImageReader operator=(ChildImageReader src) { return src; }

		explicit ChildImageReader(const MainImgReadContext* i_main_context, const ChildImgInfo i_child_info) 
			: child_info(i_child_info) , main_context(i_main_context){}

		// 获取子图片的指定行数据
		LineNumber GetChildImageLine(byte_t* data_out, uint64_t line_index, LineNumber number);

		// 获取子图片的所有数据
		LineNumber GetChildImageAllData(byte_t* out_data);

		constexpr uint64_t GetImgLineAllocSize()
		{
			return main_context->img_info.block_size_x * child_info.child_img_x_block 
				* child_info.channel * child_info.channel_byte_size;
		}

		constexpr uint64_t GetChildImgYNumber() 
		{
			return main_context->img_info.block_size_y * child_info.child_img_y_block;
		}
	};

	class MainImgReadContext	// image file context
	{
	public:
		// 读取内存数据
		explicit MainImgReadContext(const byte_t* mem_data, const uint64_t mem_data_size) 
			:file_data(mem_data), data_size(mem_data_size) 
		{
			img_info = { 0 };
			child_img_map = new std::map<std::string, ChildImgInfo>();
		}

		// 初始化图片流
		ZimgReturn InitMainContext();

		// 获取瓦片图片位图
		ZimgReturn GetChildrenImage(std::string& index, ChildImageReader& child_img_out);

		~MainImgReadContext()
		{
			child_img_map = reinterpret_cast<std::map<std::string, ChildImgInfo>*>( DeleteItem(child_img_map));
		}

	private:

		friend class ChildImageReader;

		// 总图片信息
		MainImgInfo img_info;
		// 子图片信息
		std::map<std::string, ChildImgInfo>* child_img_map;

		const byte_t* file_data;
		const uint64_t data_size;
	};
}

#endif // _ZSTD_IMG_READ_H_