#pragma once 
#include <cstdint>

namespace dq5::level {

    enum class CommandType : int32_t {
        COMMAND_GANGANIKOUZE = 0x0,
        COMMAND_BACCHIRIGANBARE = 0x1,
        COMMAND_ORENIMAKASERO = 0x2,
        COMMAND_JYUMONTUKAUNA = 0x3,
        COMMAND_INOCHIDAIZINI = 0x4,
        COMMAND_MEIREISASERO = 0x5,
        COMMAND_DEBUG = 0x6,
        COMMAND_NONE = 0x7,
    };

}
