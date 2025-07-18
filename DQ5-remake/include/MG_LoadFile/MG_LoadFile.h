
#include "SFILEINFO/SFILEINFO.h"

void* ReadFileAlloc(const char* filename, int* pSize);
void* MG_LoadFile(const char* filename, int* pSize);
static char s_zipName[1024];
static SFILEINFO* FindFileInfo(const char* name);
