#include "zstd.h"

#include "zstdImgWrite.h"
#include "zstdImgAlloc.h"

namespace zimg 
{
	bool DiskMemoryPool::InputDeskMemory1GB(byte_t* input_mem_data)
	{
		std::fstream cache_file;
		std::string item_name = cahe_name + std::to_string(CACHE_FILES.size());
		std::filesystem::path cache_path = std::filesystem::current_path() / item_name;

		if (input_mem_data == nullptr)
		{
			return false;
		}
		cache_file.open(cache_path, std::ios::binary | std::ios::out | std::ios::in);

		return true;
	}

	bool DiskMemoryPool::OutputDeskMemory1GB(byte_t* output_mem_data)
	{
		uint64_t index = CACHE_FILES.size();
		std::string item_name = cahe_name + std::to_string(index);
		std::filesystem::path cache_path = std::filesystem::current_path() / item_name;
		auto file = CACHE_FILES.end();

		file->read(reinterpret_cast<char*>(output_mem_data), MAX_CACHE_FILE_SIZE);
		file->close();

		if(std::filesystem::remove(cache_path))
		{
			return false;
		}
		CACHE_FILES.pop_back();
		return true;
	}

	DiskMemoryPool::~DiskMemoryPool()
	{
		for (auto& file : CACHE_FILES)
		{
			if (file.is_open())
			{
				file.close();
			}
		}
		CACHE_FILES.clear();
	}
}

int zWriteImgToMemory(const Zimg img, ZimgMemoryFile* mem_data)
{
	byte_t* pdata_cur					= img.data;
	ZimgFileHeader f_header				=
	{
		.FILE_HEADER_TEXT				= 1735223674llu,
		.channel						= img.channel,
		.channel_byte_size				= img.channel_byte_size,
		.x								= img.x,
		.y								= img.y,
		.compressed_data_size 			= 0
	};
	uint64_t line_size					= img.x * img.channel_byte_size * img.channel_byte_size;				// compress unit
	uint64_t buffer_size				= ZSTD_compressBound(line_size);										// zstd compress buffer size
	uint64_t output_size				= 0;

	ZimgMemoryFile* pcompress_output	= (ZimgMemoryFile*)zmalloc(8 + buffer_size);							// uint64 szie + compress buffer
	byte_t* pmem_file_cur				= (byte_t*)zmalloc((buffer_size + 8) * img.y + sizeof(ZimgFileHeader));	// pixel data cur ptr

	mem_data->size = 0;
	mem_data->data = pmem_file_cur;

	pmem_file_cur += sizeof(ZimgFileHeader);
	output_size += sizeof(ZimgFileHeader);

	for (uint64_t i = 0; i < img.y; i++)
	{
		uint64_t compress_data_size = ZSTD_compress(pcompress_output + sizeof(uint64_t), buffer_size, pdata_cur, line_size, ZSTD_lazy);

		uint32_t code = ZSTD_isError(compress_data_size);
		if (code)
		{
			printf("<LIB: ZSTD> CompressZSTD error: %s", ZSTD_getErrorName(compress_data_size));
			zfree(pcompress_output);
			zfree(pmem_file_cur);
			mem_data->data = NULL;
			return Z_COMPRESS_ERROR;
		}
		pcompress_output->size	= compress_data_size;
		memcpy(pmem_file_cur, pcompress_output, compress_data_size);
		pmem_file_cur			= pmem_file_cur + sizeof(uint64_t) + compress_data_size;
		output_size				= output_size + sizeof(uint64_t) + compress_data_size;
	}
	zfree(pcompress_output);

	mem_data->size = output_size;

	f_header.compressed_data_size = output_size - sizeof(ZimgFileHeader);
	memcpy(mem_data->data, &f_header, sizeof(ZimgFileHeader));

	mem_data->data = zrealloc(mem_data->data, output_size);
	
	return Z_OK;
}

int zWriteImgToCFile(const Zimg img, FILE* fs)
{
	ZimgMemoryFile mem_file;
	int state = zWriteImgToMemory(img, &mem_file);
	if (state != Z_OK) 
	{
		return state;
	}
	fwrite(mem_file.data, mem_file.size, 1, fs);
	zReleaseZimgMemoryFile(mem_file);
	return Z_OK;
}