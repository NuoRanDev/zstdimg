#ifndef ZSTD_IMG_WRITE
#define ZSTD_IMG_WRITE

#include "zstdImgFormat.h"
#include "zstdImgCompiler.h"

#include <vector>
#include <fstream>
#include <cstdio>
#include <filesystem>

namespace zimg
{
	constexpr uint64_t MAX_CACHE_FILE_SIZE = 1 * 1024 * 1024 * 1024; // 1GB

	// 导出缓存编号
	extern uint64_t out_index;

	class DiskMemoryPool
	{
	public:
		explicit DiskMemoryPool(std::string out_name):cahe_name(out_name){}

		bool InputDeskMemory1GB(byte_t* input_mem_data);

		bool OutputDeskMemory1GB(byte_t* output_mem_data);

		~DiskMemoryPool();
		
	private:
		// 缓存文件列表
		std::vector<std::fstream> CACHE_FILES;
		// 
		std::string cahe_name;
	};

	class MainImgWriteContext
	{
	public:
	private:
	};
};

#endif // ZSTD_IMG_WRITE