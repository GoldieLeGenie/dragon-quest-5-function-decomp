#include <fstream>
#include <cstring>
#include "args/FileUtils.h"
#include <fstream>
#include <cstdlib>

void* args::MG_LoadFile(const char* fname, int /*mode*/) {
    std::ifstream file(fname, std::ios::binary | std::ios::ate);
    if (!file) return nullptr;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    if (file.read(buffer, size)) {
        return buffer;
    }

    delete[] buffer;
    return nullptr;
}


void args::g_File_load(const char* fname, void* addr, int offset, int size) {
        std::ifstream file(fname, std::ios::binary);
        if (!file)
            return;

        file.seekg(offset, std::ios::beg);
        if (!file)
            return;

        file.read(reinterpret_cast<char*>(addr), size);
        file.close();
}

void args::FreeFileMemory_0(void* addr) {
    free(addr);  // ou delete[] si tu sais que c’était new[]
}
