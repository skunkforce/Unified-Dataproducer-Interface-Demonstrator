#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CHUNKSIZE 32768

#define LIBHANDLE void*
#define CHUNK unsigned char*
#define DEVICEHANDLE void*

DLL_EXPORT LIBHANDLE init();
DLL_EXPORT CHUNK deinit(LIBHANDLE);

// fills chunk with json
DLL_EXPORT void queryDevices(LIBHANDLE, CHUNK);

//second parameter is device name like it is in json
DLL_EXPORT DEVICEHANDLE openDevice(LIBHANDLE, unsigned char*);

DLL_EXPORT CHUNK closeDevice(LIBHANDLE, DEVICEHANDLE);

// fills chunk with json
DLL_EXPORT void getDeviceMetaData(LIBHANDLE, DEVICEHANDLE, CHUNK);

DLL_EXPORT void waitForData(LIBHANDLE, DEVICEHANDLE);

DLL_EXPORT void pushChunk(LIBHANDLE, DEVICEHANDLE, CHUNK);
DLL_EXPORT CHUNK popChunk(LIBHANDLE, DEVICEHANDLE);

#ifdef __cplusplus
}
#endif
