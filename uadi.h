#ifndef UADI_H
#define UADI_H

#include <stddef.h>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* uadi_lib_handle;
typedef void* uadi_device_handle;

typedef unsigned char* uadi_chunk_ptr;

typedef struct uadi_receive_struct{
    uadi_chunk_ptr infopack_ptr;
    uadi_chunk_ptr datapack_ptr;
};

// Error codes
#define UADI_SUCCESS 0
#define UADI_ERROR -1
#define UADI_BUFFER_TOO_SMALL -2
#define UADI_INVALID_HANDLE -3
#define UADI_NO_DATA -4
#define UADI_OUT_OF_CHUNKS -5
#define UADI_INTERNAL_ERROR -255

typedef int uadi_status;

typedef void(uadi_error_callback*)(uadi_status, void*);
typedef void(uadi_receive_callback*)(uadi_receive_struct, void*);
typedef void(uadi_release_callback*)(uadi_chunk_ptr, void*);

DLL_EXPORT uadi_status uadi_init(uadi_lib_handle* lib_handle);

DLL_EXPORT uadi_status uadi_get_meta_data(uadi_lib_handle lib_handle, uadi_chunk_ptr meta_data);

DLL_EXPORT uadi_status uadi_enumerate(uadi_lib_handle handle, uadi_chunk_ptr meta_data);

DLL_EXPORT uadi_status uadi_claim_device(uadi_lib_handle lib_handle, uadi_device_handle* device_handle, char const* device_key);

DLL_EXPORT uadi_status uadi_push_chunks(uadi_device_handle device_handle, uadi_chunk_ptr* chunk_array, size_t chunk_count);

DLL_EXPORT uadi_status uadi_register_receive_callback(uadi_device_handle device_handle, uadi_receive_callback callback, void* user_data);

DLL_EXPORT uadi_status uadi_register_error_callback(uadi_device_handle device_handle, uadi_error_callback callback, void* user_data);

DLL_EXPORT uadi_status uadi_release_device(uadi_device_handle device_handle, uadi_release_callback callback, void* user_data);

DLL_EXPORT uadi_status uadi_release_lib(uadi_lib_handle lib_handle, uadi_release_callback callback, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // UADI_H
