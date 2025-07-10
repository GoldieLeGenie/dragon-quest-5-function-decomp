#pragma once



namespace args {

	void g_File_load(const char* fname, void* addr, int offset, int size);
	void* MG_LoadFile(const char* fname, int mode);
	void FreeFileMemory_0(void* addr);


}
