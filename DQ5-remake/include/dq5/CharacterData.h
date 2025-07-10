#pragma once
#include "dq5/PlayerData1.h"
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct CharacterData {
        // Typiquement vide, ou avec des fonctions statiques uniquement
        static PlayerData1* getRecord(int playerIndex, int level);
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;
    };

}
