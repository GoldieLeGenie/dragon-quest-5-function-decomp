#include "status/getRandomVariation.h"
#include "ar/rand.h"
#include <cstdint>


int status::getRandomVariation(int value, int under, int over) {
    // Calcul du multiplicateur : (100 - under + rand(under + over + 1))
    int variation = 100 - under + ar::rand(under + over + 1);

    // Équivalent du mulhi : (1374389535 * variation * value) >> 32, puis ajustement
    int64_t product = static_cast<int64_t>(variation) * value;
    int64_t scaled = (1374389535LL * product) >> 32;

    return static_cast<int>((scaled >> 5) + (static_cast<uint64_t>(scaled) >> 63));
}