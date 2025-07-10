#include "ar/rand.h"
#include "menu/MaterielMenu_SlimeRace.h"
#include <cstdint>



int ar::rand(int n) {
    static unsigned int randSeed = 1;  
    uintptr_t noise = reinterpret_cast<uintptr_t>(&menu::MaterielMenu_SlimeRace::menuDraw);
    uintptr_t yPtr = reinterpret_cast<uintptr_t>("y");

    uintptr_t combined = noise + randSeed * yPtr + 3;
    randSeed = static_cast<uint32_t>(combined);

    if (n >= 2) {
        return static_cast<int>((randSeed >> 16) & 0x7FFF) % n;
    }
    return 0;
}


