#include <cstdint>
#include <cstdio>  // pour FILE

struct SFILEINFO {
    char* name;            // 0x00 - Pointeur vers le nom du fichier
    uint32_t offset;       // 0x04 - Offset du fichier dans l'archive
    uint32_t size;         // 0x08 - Taille du fichier
    uint16_t nameLen;      // 0x0C - Longueur du nom
    uint16_t headSize;     // 0x0E - Taille de l'en-tête

    union {
        uint32_t nameOffset; // 0x10 - Offset vers le nom dans un buffer
        FILE* file;          // 0x10 - Fichier ouvert en tant que handle
    };

};

extern int s_fileNum;
extern SFILEINFO* s_fileInfo;