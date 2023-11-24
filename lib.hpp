#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT unsigned char* chunk;
DLL_EXPORT void pushChunk(unsigned char*&);
DLL_EXPORT unsigned char* popChunk();

#ifdef __cplusplus
}
#endif