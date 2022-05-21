#ifdef LIBRARY_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif
#define RETURN_FAILURE -1

typedef struct {
	int cpuidInfo;
	int funcKeysCount;
} library_info;

LIBRARY_API int GetInfo(library_info* info);