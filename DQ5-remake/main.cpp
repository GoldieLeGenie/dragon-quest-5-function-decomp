#include "status/UseAction.h"
#include "dq5/ActionParam.h"
#include "status/HaveStatusInfo.h"
#include "status/HaveStatusInfo_0.h"
#include "status/BaseStatus.h"
#include "status/HaveEquipment.h"
#include "status/StatusChange.h"
#include "status/HaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"
#include "dq5/LevelData.h"
#include "dq5/CharacterData.h"
#include "status/StatusChangeOne.h"
#include <iostream>
#include "status/isNormalAttack.h"
#include "status/isMultiTargetEquipment.h"
#include "status/CharacterStatus.h"
#include "status/ItemData.h"
#include "status/isDoubleAction.h"
#include "status/HaveBattleStatus.h"
#include "ar/rand.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <cstring>
#include "args/ScriptEngine.h"
#include "args/ScriptGroup.h"
#include "args/ScriptObject.h"
#include "args/ScriptParam.h"
#include "args/PlacementParameter.h"
#include "args/PlacementParameterParam.h"
#include "args/DataObject.h"



int main() {
    std::cout << "DQ5 remake running!" << std::endl;
    return 0;
}

// Simule une initialisation de ActionParam
//void initActionParam() {
//    using namespace dq5::level;
//
//    // Chemin réel du fichier binaire d'action
//    ActionParam::filename_[0] = "./LEVELDATA/dq5ds_action.dat";
//    ActionParam::loadSwitch_ = 1;
//
//    // Charge l'en-tête
//    ActionParam::binary_.setupHeader(ActionParam::filename_[0], static_cast<ar::File::LoadSwitch>(ActionParam::loadSwitch_));
//    ActionParam::addr_ = ActionParam::binary_.setup();
//
//    if (!ActionParam::addr_) {
//        std::cerr << "Erreur : impossible de charger dq5ds_action_param.dat" << std::endl;
//        std::exit(1);
//    }
//
//    std::cout << "Fichier ActionParam chargé. Nombre d'actions : " << ActionParam::binary_.record_ << "\n";
//}
//
//
//
//
//void initCharacterData() {
//    using namespace dq5::level;
//
//    // Nom du fichier : dépendra de l'index, donc on peut charger un dummy d'abord (ex: index 0)
//
//    CharacterData::filename_[0] = "./LEVELDATA/dq5ds_player_level1.dat";
//
//
//    // Met à jour loadSwitch_ depuis levelData
//    CharacterData::loadSwitch_ = LevelData::getLoadType(&levelData, 28) != 0;
//
//    // Setup header pour un fichier réel
//    CharacterData::binary_.setupHeader(
//        CharacterData::filename_[0],
//        static_cast<ar::File::LoadSwitch>(CharacterData::loadSwitch_)
//    );
//
//    CharacterData::addr_ = CharacterData::binary_.setup();
//
//    if (!CharacterData::addr_) {
//        std::cerr << "Erreur : échec du setup() pour dq5ds_player_level0.dat\n";
//        std::exit(1);
//    }
//
//    std::cout << "CharacterData chargé. Enregistrements par fichier : " << CharacterData::binary_.record_ << "\n";
//}
//
//void initItemData() {
//    using namespace dq5::level;
//
//    ItemData::filename_[0] = "./LEVELDATA/dq5ds_item_list.dat";
//    ItemData::loadSwitch_ = 1;
//
//    ItemData::binary_.setupHeader(ItemData::filename_[0], static_cast<ar::File::LoadSwitch>(ItemData::loadSwitch_));
//    ItemData::addr_ = ItemData::binary_.setup();
//
//    if (!ItemData::addr_) {
//        std::cerr << "Erreur : impossible de charger dq5ds_item_list.dat\n";
//        std::exit(1);
//    }
//
//    std::cout << "ItemData chargé. Nombre d'objets : " << ItemData::binary_.record_ << "\n";
//}
//
//
//void printAllArmorItemIDs() {
//    using namespace dq5::level;
//
//    std::cout << "== Liste des items de type ARMOR ==\n";
//    for (int id = 0; id < ItemData::binary_.record_; ++id) {
//        auto type = status::UseItem::getEquipItemType(id);
//        if (type == ItemType::ITEM_ACCESSORY) {
//            std::cout << "ID ARMOR: " << id << "\n";
//        }
//    }
//}





//int main() {
//    initCharacterData();  // ← obligatoire
//    int playerIndex = 5;
//    int level = 3;
//
//    dq5::level::PlayerData1* pdata = status::HaveStatus::getLevelupData(playerIndex, level);
//
//    if (pdata) {
//        std::cout << "Level: " << (int)pdata->level << "\n";
//        std::cout << "HP: " << pdata->HP << ", MP: " << pdata->MP << "\n";
//        std::cout << "STR: " << pdata->strength << ", AGI: " << pdata->agility << "\n";
//        std::cout << "VITALITY: " << pdata->vitality << ", Intelligence: " << pdata->intelligence << "\n";
//        std::cout << "Luck: " << pdata->luck << "\n";
//
//    }
//    else {
//        std::cout << "Aucune donnée trouvée.\n";
//    }
//
//    return 0;
//}
//



//int main() {
//    using namespace status;
//    initItemData();
//
//    HaveStatusInfo_0 character{};
//    character.characterType_ = dq5::level::CharacterType::PLAYER;
//
//    // Setup des stats de base
//    character.haveStatus_.baseStatus_.strength_ = 50;
//    character.haveStatus_.baseStatus_.protection_ = 40;
//    character.haveStatus_.baseStatus_.hpMax_ = 1100;
//    character.haveStatus_.baseStatus_.mpMax_ = 100;
//    character.haveStatus_.gold_ = 1234;
//
//    character.haveStatus_.setHp(80);
//    character.haveStatus_.setMp(10);
//
//    character.flag2_.setValue(0x001C0000); 
//    character.flag_.setValue(0x00000400);  
//
//    static HaveItem simulatedInventory;
//    simulatedInventory.itemArray_[0].index_ = 1;
//    simulatedInventory.itemMax_ = 12;
//
//    character.haveEquipment_.haveItem_ = &simulatedInventory;
//
//    character.haveEquipment_.strength_ = 20;
//    character.haveEquipment_.attack_ = 15;
//    character.haveEquipment_.defence_ = 30;
//
//    std::cout << "isDeath: " << HaveStatusInfo::isDeath(&character) << "\n";
//    std::cout << "getHp: " << HaveStatusInfo::getHp(&character) << "\n";
//    std::cout << "getGold: " << HaveStatusInfo::getGold(&character) << "\n";
//
//    HaveStatusInfo::clearHpInBattle(&character);
//    std::cout << "After clearHpInBattle: " << character.hp_[0] << ", " << character.hp_[1] << "\n";
//
//    HaveStatusInfo::clearMpInBattle(&character);
//    std::cout << "After clearMpInBattle: " << character.mp_[0] << ", " << character.mp_[1] << "\n";
//
//    HaveStatusInfo::clearStatusChangeInBattle(&character);
//    std::cout << "StatusChange (BeforeAction): " << character.status_[0].get() << "\n";
//
//    std::cout << "isMultiAttack: " << HaveStatusInfo::isMultiAttack(&character) << "\n";
//
//    HaveStatusInfo::setMultiAttack(&character, false);
//    std::cout << "After clear: isMultiAttack: " << HaveStatusInfo::isMultiAttack(&character) << "\n";
//
//    HaveStatusInfo::setMultiAttack(&character, true);
//    std::cout << "After set: isMultiAttack: " << HaveStatusInfo::isMultiAttack(&character) << "\n";
//
//    HaveStatusInfo::setImmediateDeathItem(&character, true);
//    std::cout << std::hex << "flag2 (after set true): " << character.flag2_.get() << "\n";
//    HaveStatusInfo::setImmediateDeathItem(&character, false);
//    std::cout << "flag2 (after set false): " << character.flag2_.get() << "\n";
//
//    // Test attaque / défense
//    std::cout << std::dec;
//    std::cout << "getAttack: " << HaveStatusInfo::getAttack(&character, 0) << "\n";
//    std::cout << "getDefence: " << HaveStatusInfo::getDefence(&character, 0) << "\n";
//
//    // Kaishin / Tsukon
//    std::cout << "isKaishin: " << HaveStatusInfo::isKaishin(&character) << "\n";
//    std::cout << "isTsukon1: " << HaveStatusInfo::isTsukon1(&character) << "\n";
//    std::cout << "isTsukon2: " << HaveStatusInfo::isTsukon2(&character) << "\n";
//
//    // HP Max
//    std::cout << "getHpMax: " << HaveStatusInfo::getHpMax(&character) << "\n";
//    
//    return 0;
//}
