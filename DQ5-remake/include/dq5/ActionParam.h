#pragma once
#include <cstdint>
#include "args/ExcelBinaryData.h"


namespace dq5::level {

    struct ActionParam {
      
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;


        enum DEFENCETYPE : int32_t
        {
            DEFENCETYPE_TYPE_A = 0x0,
            DEFENCETYPE_TYPE_B = 0x1,
            DEFENCETYPE_TYPE_C = 0x2,
            DEFENCETYPE_TYPE_D = 0x3,
            DEFENCETYPE_TYPE_E = 0x4,
            DEFENCETYPE_TYPE_F = 0x5,
            DEFENCETYPE_TYPE_Z = 0x6,
            DEFENCETYPE_MAX = 0x7,
        };
        enum class DEFENCE : int32_t {
            DEFENCE_MERA = 0x00,
            DEFENCE_GIRA = 0x01,
            DEFENCE_IO = 0x02,
            DEFENCE_HYADO = 0x03,
            DEFENCE_BAGI = 0x04,
            DEFENCE_DEIN = 0x05,
            DEFENCE_ZAKI = 0x06,
            DEFENCE_MEGANTE = 0x07,
            DEFENCE_NIFRAM = 0x08,
            DEFENCE_MANUSA = 0x09,
            DEFENCE_MEDAPANI = 0x0A,
            DEFENCE_RARIHO = 0x0B,
            DEFENCE_MAHOTOON = 0x0C,
            DEFENCE_MAHOTORA = 0x0D,
            DEFENCE_RUKANI = 0x0E,
            DEFENCE_SUKARA = 0x0F,
            DEFENCE_FIRE = 0x10,
            DEFENCE_BLIZZARD = 0x11,
            DEFENCE_POISON = 0x12,
            DEFENCE_VENOM = 0x13,
            DEFENCE_PARALYSIS = 0x14,
            DEFENCE_REST = 0x15,
            DEFENCE_ALL = 0x16,
            DEFENCE_HALF = 0x17,
            DEFENCE_SUCCESS_4TH = 0x18,
            DEFENCE_SUCCESS_6TH = 0x19,
            DEFENCE_SUCCESS_8TH = 0x1A,
            DEFENCE_SUCCESS_3_4 = 0x1B,
            DEFENCE_SUCCESS_3_8 = 0x1C,
            DEFENCE_SUCCESS_5_8 = 0x1D,
            DEFENCE_ENCOUNT = 0x1E,
            DEFENCE_RARIHOMA = 0x1F,
            DEFENCE_RUKANAN = 0x20,
            DEFENCE_MAX = 0x21
        };
        enum class ACTIONTYPE : int32_t {
            ACTIONTYPE_NONE = 0x0,
            ACTIONTYPE_DAMAGE = 0x1,
            ACTIONTYPE_KAIFUKU = 0x2,
            ACTIONTYPE_SOKUSHI = 0x3,
            ACTIONTYPE_FUKKATSU = 0x4,
            ACTIONTYPE_MP_ADD = 0x5,
            ACTIONTYPE_MP_SUB = 0x6,
            ACTIONTYPE_ST_CLR = 0x7,
            ACTIONTYPE_NAKAMAYOBI = 0x8,
            ACTIONTYPE_PARTY_CHANGE = 0x9,
            ACTIONTYPE_SAKUSENHENKOU = 0xA,
            ACTIONTYPE_DISAPPEAR = 0xB,
            ACTIONTYPE_STR_PLUS = 0xC,
            ACTIONTYPE_SPD_PLUS = 0xD,
            ACTIONTYPE_DEF_PLUS = 0xE,
            ACTIONTYPE_INT_PLUS = 0xF,
            ACTIONTYPE_MAXHP_PLUS = 0x10,
            ACTIONTYPE_MAXMP_PLUS = 0x11,
            ACTIONTYPE_ASTORON = 0x12,
            ACTIONTYPE_MAHI = 0x13,
            ACTIONTYPE_NEMURI = 0x14,
            ACTIONTYPE_MANUSA = 0x15,
            ACTIONTYPE_BAIKIRUTO = 0x16,
            ACTIONTYPE_DF_HENKA = 0x17,
            ACTIONTYPE_BUKIMINAHIKARI = 0x18,
            ACTIONTYPE_FUBAHA = 0x19,
            ACTIONTYPE_MAHOKANTA = 0x1A,
            ACTIONTYPE_MOSYASU = 0x1B,
            ACTIONTYPE_TAMEKEI = 0x1C,
            ACTIONTYPE_MAHOTON = 0x1D,
            ACTIONTYPE_DORAGORAM = 0x1E,
            ACTIONTYPE_KONRAN = 0x1F,
            ACTIONTYPE_PATH_1 = 0x20,
            ACTIONTYPE_DOKU = 0x21,
            ACTIONTYPE_MOUDOKU = 0x22,
            ACTIONTYPE_CLOSEDOOR = 0x23,
            ACTIONTYPE_CURSE1 = 0x24,
            ACTIONTYPE_CURSE2 = 0x25,
            ACTIONTYPE_CURSE3 = 0x26,
            ACTIONTYPE_MAHOKITE = 0x27,
            ACTIONTYPE_DEFENCE = 0x28,
            ACTIONTYPE_FIZZLEZONE = 0x29,
            ACTIONTYPE_TIMESTOP = 0x2A,
            ACTIONTYPE_TOHEROSU = 0x2B,
            ACTIONTYPE_SINOBIASI = 0x2C,
            ACTIONTYPE_CURSE4 = 0x2D,
            ACTIONTYPE_CURSE5 = 0x2E,
            ACTIONTYPE_NORMAL_MAMONONOESA = 0x2F,
            ACTIONTYPE_MSG_ONLY = 0x30,
            ACTIONTYPE_MSG_SUCCESS = 0x31,
            ACTIONTYPE_MSG_FAILED = 0x32,
            ACTIONTYPE_MAX = 0x33
        };
        unsigned long actionMes;         // 0x00
        unsigned long playerSuccess;     // 0x04
        unsigned long monsterSuccess;    // 0x08
        unsigned long playerFailed;      // 0x0C
        unsigned long monsterFailed;     // 0x10
        unsigned long endMes;            // 0x14
        unsigned long menuMes;           // 0x18
        unsigned short index;             // 0x1C
        unsigned short effectFriend;      // 0x1E
        unsigned short effectEnemy;       // 0x20
        unsigned short effectLap;         // 0x22
        unsigned short MonsterMin;        // 0x24
        unsigned short MonsterMax;        // 0x26
        unsigned short PlayerMin;         // 0x28
        unsigned short PlayerMax;         // 0x2A
        unsigned short actionSE;          // 0x2C
        unsigned short startSE;           // 0x2E
        unsigned char type;               // 0x30
        unsigned char magictype2;         // 0x31
        unsigned char useMP;              // 0x32
        unsigned char fool;               // 0x33
        unsigned char human;              // 0x34
        unsigned char god;                // 0x35
        unsigned char typeID;             // 0x36
        unsigned char magictype;          // 0x37
        char byte_1;              // 0x38
        char byte_2;              // 0x39
        char byte_3;              // 0x3A
        char byte_4;              // 0x3B
        char byte_5;              // 0x3C
        char byte_6;              // 0x3D
        char byte_7;              // 0x3E
        unsigned char dummy0;             // 0x3F
        
        

    };
};

