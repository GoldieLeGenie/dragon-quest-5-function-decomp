#include "status/getRandomVariation.h"
#include "ar/rand.h"
#include <cstdint>


int status::getRandomVariation(int value, int under, int over) {
    uint64_t variation = 100 - under + ar::rand(under + over + 1);
    uint64_t product = variation * value;
    uint64_t scaled = (1374389535ULL * product) >> 32;

    return static_cast<int>((scaled >> 5) + (static_cast<uint32_t>(scaled) >> 31));
}