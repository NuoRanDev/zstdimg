#ifndef _ZSTD_IMG_ALLOC_H_
#define _ZSTD_IMG_ALLOC_H_

#include "zstdImgCompiler.h"
#include <stdlib.h>

namespace zimg
{
	constexpr void* DeleteItem(void* ptr)
	{
		if (ptr != nullptr)
		{
			delete ptr;
		}
		return nullptr;
	}
};

#endif // _ZSTD_IMG_ALLOC_H_