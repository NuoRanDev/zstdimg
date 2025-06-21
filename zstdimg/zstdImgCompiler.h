#ifndef _ZSTD_IMG_COMPILER_H_
#define _ZSTD_IMG_COMPILER_H_

// Export function data
#if defined(_MSC_VER) // MSVC
#define ZIMG_EXPORT_HEAD __declspec(dllexport)
#elif defined(__GNUC__) //GCC
#define EXPORT_HEAD __attribute__((visibility("default")))
#endif // API EXPORT IS END

#ifdef __cplusplus
#define ZIMG_EXTERN_C_STMT				extern"C"
#else
#define EXTERN_C_STMT
#endif // __cplusplus

#ifndef CALL
#if defined(WIN32) || defined(_WIN32)
#define CALL __stdcall
#else
#define CALL __cdecl
#endif
#endif

#define ZIMG_EXPORT_C_SHARP_API(ret_type) ZIMG_EXTERN_C_STMT ZIMG_EXPORT_HEAD ret_type CALL

#endif // _ZSTD_IMG_COMPILER_H_