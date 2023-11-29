/**
 * @file uadi.h
 * @brief Header file for Unified Abstract Dataproducer Interface (UaDI) library.
 * @author Stephan Bökelmann
 * @email sboekelmann@ep1.rub.de
 *
 * This file defines the API for interacting with various types of data producers.
 * It includes functions for initializing the library, enumerating available data producers,
 * claiming and releasing devices, managing data chunks, and waiting for data.
 * Detailed error codes and data management policies are provided for robust integration.
 */

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

/** 
 * @brief Handle for the connection-id to the library instance.
 *
 * This is a void pointer to an instance that implements the DLL interface.
 * The library is responsible for allocating and deallocating this handle.
 */
typedef void* uadi_lib_handle;

/** 
 * @brief Handle for a device instance.
 *
 * This pointer points to an instance that implements the interface of an abstract data producer.
 * The library is responsible for managing the lifecycle of this handle.
 * It gets created by claiming a device and gets destroyed after a device has been released by the consumer.
 */
typedef void* uadi_device_handle;

/** 
 * @brief Pointer to a chunk of memory.
 *
 * This is used to manage chunks of memory for data storage. Chunks ought to be contiguous
 * in memory and at least 128KB in size. The client is responsible for allocating and
 * deallocating these chunks.
 */
typedef unsigned char* uadi_chunk_ptr;

/** 
 * @brief Structure to receive data from the library.
 *
 * This structure is used when popping chunks of data from the library.
 * It contains pointers to information and data packets. The format of data packets
 * is an array of floats. Information packets are JSON strings.
 * @see uadi_pop_chunk
 */
struct uadi_receive_struct {
    uadi_chunk_ptr infopack_ptr;
    uadi_chunk_ptr datapack_ptr;
};

/** 
 * @brief Typedef for uadi_receive_struct.
 */
typedef struct uadi_receive_struct uadi_receive_struct;

/** 
 * @brief Typedef for pointer to uadi_receive_struct.
 */
typedef uadi_receive_struct* uadi_receive_struct_ptr;

// Error codes
#define UADI_SUCCESS 0
#define UADI_ERROR -1
#define UADI_BUFFER_TOO_SMALL -2
#define UADI_INVALID_HANDLE -3
#define UADI_NO_DATA -4
#define UADI_OUT_OF_CHUNKS -5

/**
 * @brief Initialize the library and get a library handle.
 * 
 * @return uadi_lib_handle A handle to the library instance. Returns nullptr if buffer is too small or other initialization errors occur.
 */
DLL_EXPORT uadi_lib_handle uadi_init();

/**
 * @brief Initialize the library and get a library handle.
 * 
 * @param handle Handle to the library instance, previously acquired by uadi_init().
 * @param buffer Pointer to a memory buffer for metadata. The buffer should be at least 128KB.
 * @param buffer_length Length of the provided memory buffer.
 * @return int UADI_SUCCESS on success, UADI_BUFFER_TOO_SMALL if buffer is too small, or other non-zero error codes on failure.
 */
DLL_EXPORT int uadi_get_metadata(uadi_lib_handle handle, char* buffer, size_t buffer_length);

/**
 * @brief Enumerate available data producers.
 * 
 * @param handle Library handle.
 * @param buffer Pointer to a memory buffer for the enumeration list. Must be at least 128KB.
 * @param buffer_size Size of the provided buffer.
 * @return int UADI_SUCCESS on success, UADI_BUFFER_TOO_SMALL if buffer is too small, or other non-zero error codes on failure.
 */
DLL_EXPORT int uadi_enumerate(uadi_lib_handle handle, char* buffer, size_t buffer_size);

/**
 * @brief Claim exclusive access to a data producer.
 * 
 * @param device_handle Handle to the library instance.
 * @param device_key Null-terminated string containing the device key.
 * @param chunk_array Pointer to an array of memory chunks. Chunks should be contiguous and at least 128KB each.
 * @param chunk_count Number of chunks in the array.
 * @return uadi_device_handle A handle to the claimed device, or nullptr on failure (e.g., invalid handle, device not available).
 */
DLL_EXPORT uadi_device_handle uadi_claim_device(uadi_lib_handle handle, const char* device_key, uadi_chunk_ptr* chunk_array, size_t chunk_count);

/**
 * @brief Push a set of chunks to a device's empty chunk queue.
 * 
 * @param device_handle Handle to the device.
 * @param chunk_array Pointer to an array of memory chunks.
 * @param chunk_count Number of chunks in the array.
 * @return int UADI_SUCCESS on success, or UADI_INVALID_HANDLE if deviceHandle is invalid.
 */
DLL_EXPORT int uadi_push_chunks(uadi_device_handle device_handle, uadi_chunk_ptr* chunk_array, size_t chunk_count);

/**
 * @brief Pop a chunk of data from the device.
 * 
 * @param device_handle Handle to the device.
 * @param receive_struct_ptr Pointer to a uadi_receive_struct to be filled by the library.
 * @return int UADI_SUCCESS on success, UADI_NO_DATA if no data is available, or other non-zero error codes on failure.
 */
DLL_EXPORT int uadi_pop_chunk(uadi_device_handle device_handle, uadi_receive_struct_ptr receive_struct_ptr);

/**
 * @brief Wait for data in a blocking manner.
 * 
 * @param microseconds Time to wait in microseconds.
 * @return int UADI_SUCCESS if data is available, UADI_NO_DATA if timeout occurred without data, or other non-zero error codes on failure.
 */
DLL_EXPORT int uadi_wait_blockingly_for_data(int microseconds);

/**
 * @brief Attempt to release a claimed device.
 * 
 * @param device_handle Handle to the device.
 * @return nullptr on successful release or a valid uadi_chunk_ptr if the output-queue isn't yet empty.
 */
DLL_EXPORT uadi_chunk_ptr uadi_try_release_device(uadi_device_handle device_handle);

#ifdef __cplusplus
}
#endif

#endif // UADI_H
