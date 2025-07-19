#include "status/HaveStatusInfo.h"
#include "status/HaveStatusInfo_0.h"
#include "status/HaveStatus.h"
#include "dq5/ActionParam.h"
#include "dq5/CharacterType.h"
#include "ar/BitFlag.h"
#include "status/HaveEquipment.h"
#include "status/UseItem.h"
#include "status/BaseStatus.h"
#include "status/GameFlag.h"
#include <cstdint>
#include <cstring> 


int status::HaveStatusInfo::globalFlag_ = 0;


bool status::HaveStatusInfo::isDeath(const HaveStatusInfo_0* self)
{
    return status::HaveStatus::getHp(&self->haveStatus_) == 0;
}

uint16_t status::HaveStatusInfo::getHp(const HaveStatusInfo_0* self) {
    return status::HaveStatus::getHp(&self->haveStatus_);
}

void status::HaveStatusInfo::clearHpInBattle(HaveStatusInfo_0* self)
{
  
    status::HaveStatus* statusPtr = &self->haveStatus_;

    // Copie les PV actuels dans les 4 slots de `hp_[]`
    int hp = status::HaveStatus::getHp(statusPtr);

    self->hp_[0] = static_cast<int16_t>(hp);
    self->hp_[1] = static_cast<int16_t>(hp);
    self->hp_[2] = static_cast<int16_t>(hp);
    self->hp_[3] = static_cast<int16_t>(hp);
}


void status::HaveStatusInfo::clearMpInBattle(HaveStatusInfo_0* self)
{

    // Réinitialise les 4 emplacements de MP
    self->mp_[0] = 0;
    self->mp_[1] = 0;
    self->mp_[2] = 0;
    self->mp_[3] = 0;

    // Si noDamage_ est faux, on appelle addBaseMp jusqu'à 4 fois
    if (!self->noDamage_)
    {
        status::HaveStatus::addBaseMp(&self->haveStatus_, 0);
        if (!self->noDamage_)
        {
            status::HaveStatus::addBaseMp(&self->haveStatus_, 0);
            if (!self->noDamage_)
            {
                status::HaveStatus::addBaseMp(&self->haveStatus_, 0);
                if (!self->noDamage_)
                {
                    status::HaveStatus::addBaseMp(&self->haveStatus_, 0);
                }
            }
        }
    }
}


int status::HaveStatusInfo::getGold(const HaveStatusInfo_0* self) {
    return self->haveStatus_.gold_;
}


void status::HaveStatusInfo::clearStatusChangeInBattle(HaveStatusInfo_0* self) {
    for (int i = 0; i < 4; ++i) {
        self->status_[i].reset();
    }

    status::HaveStatusInfo::setStatusChangeInBattle(self, DiffStatus::BeforeAction);
    status::HaveStatusInfo::setStatusChangeInBattle(self, DiffStatus::ExecuteAction);
    status::HaveStatusInfo::setStatusChangeInBattle(self, DiffStatus::ResultAction);
    status::HaveStatusInfo::setStatusChangeInBattle(self, DiffStatus::SpecialAction);
}


void status::HaveStatusInfo::setStatusChangeInBattle(HaveStatusInfo_0* self, DiffStatus status) {

    ar::Flag32& flag = self->status_[static_cast<int>(status)];
    flag.reset();

    status::StatusChange* change = &self->statusChange_;

    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MANUSA))
        flag.set(0);
    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHOTON))
        flag.set(1);
    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI))
        flag.set(2);
    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_KONRAN))
        flag.set(3);
    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI))
        flag.set(4);
    if (status::StatusChange::isEnable(change, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU))
        flag.set(5);
}

void status::HaveStatusInfo::setImmediateDeathItem(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(29);
    else
        self->flag2_.clear(29);
}

bool status::HaveStatusInfo::isImmediateDeathItem(HaveStatusInfo_0* self)
{
    return self->flag2_.test(29);
}


uint16_t status::HaveStatusInfo::getAttack(HaveStatusInfo_0* self, int effect)
{
    if (!self)
        return 0;

    // Cas spécial : ne prendre en compte que la force brute
    if ((effect & 1) != 0)
        return self->haveStatus_.baseStatus_.strength_;

    // Si un équipement spécial (ID 66) est équipé, force la valeur d’attaque à 69
    if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 66))
        return 69;

    // Force de base
    uint16_t strength = self->haveStatus_.baseStatus_.strength_;

    // Met à jour les effets d’équipement
    status::HaveEquipment::calcEffect(&self->haveEquipment_);

    // Cumul force de base + bonus de force équipement
    uint16_t totalStrength = strength + self->haveEquipment_.strength_;

    // Si personnage joueur, limite la force à 255 (uint8)
    if (self->characterType_ == dq5::level::CharacterType::PLAYER)
    {
        if (totalStrength >= 0xFF)
            totalStrength = 0xFF;
    }

    // Recalcul des effets d’équipement (probablement utile si modifié dans le bloc précédent)
    status::HaveEquipment::calcEffect(&self->haveEquipment_);

    // Total attaque = bonus équipement + force
    uint16_t totalAttack = totalStrength + self->haveEquipment_.attack_;

    // Si le total est nul, retourne 0
    if (totalAttack == 0)
        return 0;

    // Retourne le total, limité à 9999
    return (totalAttack <= 9999) ? totalAttack : 9999;
}



uint16_t status::HaveStatusInfo::getDefence(HaveStatusInfo_0* self, int effect)
{
    if (!self)
        return 0;

    // Cas spécial : utiliser la défense de base uniquement
    if ((effect & 1) != 0)
        return self->haveStatus_.baseStatus_.protection_;

    // Certains équipements désactivent totalement la défense
    if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 23) ||
        status::HaveEquipment::isEquipment(&self->haveEquipment_, 117))
        return 0;

    // Cas spécial : si équipement 66 est équipé → défense forcée à 54
    if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 66))
        return 54;

    // Récupère la défense de base
    uint16_t baseDef = self->haveStatus_.baseStatus_.protection_;

    // Applique les effets d'équipement
    status::HaveEquipment::calcEffect(&self->haveEquipment_);

    // Clamp protection à 255 si le personnage est joueur
    if (self->characterType_ == dq5::level::CharacterType::PLAYER)
    {
        if (baseDef > 255)
            baseDef = 255;
    }

    // Total avec défense équipement
    int16_t total = static_cast<int16_t>(baseDef + self->haveEquipment_.defence_);

    // Appliquer effet de changement de défense s’il est actif
    if (status::StatusChange::isEnable(&self->statusChange_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DF_HENKA))
    {
        total += self->defenceChange_;
    }

    if (total < 1)
        return 0;
    else if (total > 9998)
        return 9999;

    return static_cast<uint16_t>(total);
}


bool status::HaveStatusInfo::isKaishin(HaveStatusInfo_0* self)
{
    return self->flag2_.test(18);
}


bool status::HaveStatusInfo::isMultiAttack(HaveStatusInfo_0* self) {
    uint8_t byte1 = static_cast<uint8_t>((self->flag_.get() >> 8) & 0xFF);
    return (byte1 >> 2) & 0x1;
}


void status::HaveStatusInfo::setMultiAttack(HaveStatusInfo_0* self, bool flag) {
    constexpr int MULTI_ATTACK_BIT = 10;
    if (flag)
        self->flag_.set(MULTI_ATTACK_BIT);
    else
        self->flag_.clear(MULTI_ATTACK_BIT);
}

uint16_t status::HaveStatusInfo::getHpMax(const HaveStatusInfo_0* self) {
    uint16_t hpMax = status::HaveStatus::getHpMax(&self->haveStatus_);

    if (hpMax > 9999)
        hpMax = 9999;

    if (self->characterType_ == dq5::level::CharacterType::PLAYER && hpMax > 999)
        return 999;

    return hpMax;
}

void status::HaveStatusInfo::setMp(HaveStatusInfo_0* self, uint16_t mp) {
    if (!self->noDamage_) {
        status::HaveStatus::setMp(&self->haveStatus_, mp);
    }
}


uint16_t status::HaveStatusInfo::getMp(const HaveStatusInfo_0* self) {
    return status::HaveStatus::getMp(&self->haveStatus_);
}



void status::HaveStatusInfo::HaveStatusinfo(HaveStatusInfo_0* self) {
    std::memset(self->status_, 0, sizeof(self->status_));

    /*CONSTRUCTOR status::ActionDefence()*/
    status::ActionDefence v4 = self->actionDefence_;

    self->noDamage_ = 0;
    self->defenceChange_ = 0;
    
    self->flag_.reset();
    self->flag2_.reset();
    self->testFlag_.reset();
    self->battleFlag_.reset();

    status::HaveStatus::HaveStatus(self->haveStatus_);
    status::HaveItem::HaveItem(self->haveItem_);
    status::HaveAction::HaveAction(self->haveAction_);
    status::HaveEquipment::HaveEquipment(self->haveEquipment_);
    status::StatusChange::StatusChange(self->statusChange_);

    self->commandTypeDebug_ = dq5::level::CommandTypeDebug::DEFENCE_ONLY;;
}

void status::HaveStatusInfo::setHp(HaveStatusInfo_0* self, uint16_t hp)
{
    if (!self->noDamage_)
    {
        status::HaveStatus::setHp(&self->haveStatus_, hp);
       /* status::HaveStatusInfo::execDeath(self);*/
    }
}


void status::HaveStatusInfo::setDeath(HaveStatusInfo_0* self) {
    status::HaveStatusInfo::setHp(self, 0);
}


int status::HaveStatusInfo::setItemEquipment(HaveStatusInfo_0* self,int itemIndex) {
    if (itemIndex == 0)
        return -1;

    int itemPos = status::HaveItem::add(&self->haveItem_, itemIndex);  
    status::HaveStatusInfo::setEquipment(self,itemPos);                   
    return itemPos;
}


void status::HaveStatusInfo::setEquipment(HaveStatusInfo_0* self,int index) {
    if (self->allEquipment_ || !status::BaseHaveItem::isEquipment(&self->haveItem_, index))
        return;

    int item = status::BaseHaveItem::getItem(&self->haveItem_, index);

    switch (status::UseItem::getEquipItemType(item)) {
    case dq5::level::ItemType::ITEM_WEAPON: {
        int equip = status::HaveItem::getEquipmentItem(&self->haveItem_, dq5::level::ItemType::ITEM_WEAPON, 0);
        status::BaseHaveItem::resetEquipmentWithItemIndex(&self->haveItem_, equip);
        break;
    }
    case dq5::level::ItemType::ITEM_ARMOR: {
        int equip = status::HaveItem::getEquipmentItem(&self->haveItem_, dq5::level::ItemType::ITEM_ARMOR, 0);
        status::BaseHaveItem::resetEquipmentWithItemIndex(&self->haveItem_, equip);
        break;
    }
    case dq5::level::ItemType::ITEM_SHIELD: {
        int equip = status::HaveItem::getEquipmentItem(&self->haveItem_, dq5::level::ItemType::ITEM_SHIELD, 0);
        status::BaseHaveItem::resetEquipmentWithItemIndex(&self->haveItem_, equip);
        break;
    }
    case dq5::level::ItemType::ITEM_HELMET: {
        int equip = status::HaveItem::getEquipmentItem(&self->haveItem_, dq5::level::ItemType::ITEM_HELMET, 0);
        status::BaseHaveItem::resetEquipmentWithItemIndex(&self->haveItem_, equip);
        break;
    }
    default:
        break;
    }

    status::BaseHaveItem::setEquipment(&self->haveItem_, index);
}


void status::HaveStatusInfo::resetEquipment(HaveStatusInfo_0* self, int index)
{
    status::HaveItem* haveItem = &self->haveItem_;

    int itemId = status::BaseHaveItem::getItem(haveItem, index);

    status::HaveEquipment::resetEquipment(&self->haveEquipment_, itemId);
    status::BaseHaveItem::resetEquipment(haveItem, index);
}



int status::HaveStatusInfo::getDaughterCharaIndex() {
    // Teste si le flag global 0xEB (235) est activé
    if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEB)) {
        // Si non, teste le flag 0xEC (236)
        if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEC)) {
            // Si encore non, teste le flag 0x259 (601)
            if (status::GameFlag::check(&status::g_GlobalFlag, 0x259)) {
                return 234;
            }
            return 216;
        }
        return 217;
    }
    return 216;
}

int status::HaveStatusInfo::getSonCharaIndex() {
    if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEB)) {
        if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEC)) {
            if (status::GameFlag::check(&status::g_GlobalFlag, 0x259)) {
                return 233;
            }
            return 214;
        }
        return 215;
    }
    return 214;
}


int status::HaveStatusInfo::getDaughterIconIndex() {
    if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEB)) {
        if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEC)) {
            if (status::GameFlag::check(&status::g_GlobalFlag, 0x259)) {
                return 18;
            }
            return 16;
        }
        return 17;
    }
    return 16;
}

int status::HaveStatusInfo::getSonIconIndex() {
    if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEB)) {
        if (!status::GameFlag::check(&status::g_GlobalFlag, 0xEC)) {
            if (status::GameFlag::check(&status::g_GlobalFlag, 0x259)) {
                return 15;
            }
            return 13;
        }
        return 14;
    }
    return 13;
}

void status::HaveStatusInfo::setNearDeath(HaveStatusInfo_0* self)
{
    dq5::level::CharacterType characterType; 
    unsigned int HpMax; 
    unsigned int MaxValue; 

    characterType = self->characterType_;
    HpMax = status::HaveStatus::getHpMax(&self->haveStatus_);
    MaxValue = 999;
    if (HpMax < 999)
        MaxValue = HpMax;
    if (characterType != dq5::level::CharacterType::NONE)
        MaxValue = HpMax;
    status::HaveStatusInfo::setHp(self, (MaxValue >> 2) - 1);
}


void status::HaveStatusInfo::recovery(HaveStatusInfo_0* self)
{
    status::HaveStatus* p_haveStatus; 
    uint16_t HpMax;
    uint16_t MpMax;

    p_haveStatus = &self->haveStatus_;
    HpMax = status::HaveStatus::getHpMax(&self->haveStatus_);
    status::HaveStatus::setHp(p_haveStatus, HpMax);
    MpMax = status::HaveStatus::getMpMax(p_haveStatus);
    status::HaveStatus::setMp(p_haveStatus, MpMax);
    status::StatusChange::clear(&self->statusChange_);
}
void status::HaveStatusInfo::revival(HaveStatusInfo_0* self)
{
    status::HaveStatus* p_haveStatus; 
    uint16_t HpMax; 
    uint16_t MpMax; 

    p_haveStatus = &self->haveStatus_;
    HpMax = status::HaveStatus::getHpMax(&self->haveStatus_);
    status::HaveStatus::setHp(p_haveStatus, HpMax);
    MpMax = status::HaveStatus::getMpMax(p_haveStatus);
    status::HaveStatus::setMp(p_haveStatus, MpMax);
}

void status::HaveStatusInfo::addHp(HaveStatusInfo_0* self,int hp) {
    if (!self->noDamage_) {
        status::HaveStatus::addBaseHp(&self->haveStatus_, hp);

        if (status::HaveStatus::getHp(&self->haveStatus_) == 0) {
            if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 156)) {
                self->battleFlag_.set(5);   // 0x20
                self->battleFlag_.set(9);   // 0x200
            }

            if (status::BaseHaveItem::isItem(&self->haveItem_, 184)) {
                self->battleFlag_.set(10);  // 0x400
            }

            self->flag2_.set(28);           // 0x10000000

            if (self->battleFlag_.test(4)) { // 0x10
                status::HaveStatus::addBaseHp(&self->haveStatus_, 1);
            }
        }

        /*NEED TO DO
        status::HaveStatusInfo::execDeath(self);*/
    }
}

uint16_t status::HaveStatusInfo::getStrength(HaveStatusInfo_0* self, int effect) {
    uint16_t strength = self->haveStatus_.baseStatus_.strength_;

    if ((effect & 1) == 0) {
        status::HaveEquipment::calcEffect(&self->haveEquipment_);
        strength += self->haveEquipment_.strength_;

        if (self->characterType_ == dq5::level::CharacterType::PLAYER) {
            if (strength >= 0xFF)
                strength = 0xFF;
            return static_cast<uint8_t>(strength);
        }
    }

    return strength;
}

#include <cstdint>

uint8_t status::HaveStatusInfo::getAgility(HaveStatusInfo_0* self, int effect) {
    status::HaveStatus* p_haveStatus = &self->haveStatus_;
    uint8_t agility = status::HaveStatus::getAgility(p_haveStatus);

    if ((effect & 1) != 0) {
        return agility;
    }
    else {
        status::HaveEquipment::calcEffect(&self->haveEquipment_);

        bool isEquipment = status::HaveEquipment::isEquipment(&self->haveEquipment_, 155);
        int16_t baseAgility = status::HaveStatus::getAgility(p_haveStatus);

        status::HaveEquipment::calcEffect(&self->haveEquipment_);

        int16_t modifiedAgility = self->haveEquipment_.agility_ + (isEquipment ? 2 * baseAgility : baseAgility);

        int16_t agilityChange = 0;

        if (status::StatusChange::isEnable(&self->statusChange_,
            dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SPD_PLUS)) {
            agilityChange = self->agilityChange_;
        }

        uint16_t finalAgility = static_cast<uint16_t>(modifiedAgility + agilityChange);

        if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 113)) {
            finalAgility = 0;
        }
        if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 66)) {
            finalAgility = 43;
        }

        if (finalAgility >= 0xFF) {
            finalAgility = 0xFF;
        }

        return static_cast<uint8_t>(finalAgility);
    }
}

uint16_t status::HaveStatusInfo::getProtection(HaveStatusInfo_0* self, int effect) {
    bool checkCondition = (effect & 1) == 0;

    if (checkCondition) {
        effect = static_cast<int>(self->characterType_);
    }

    uint16_t protection = self->haveStatus_.baseStatus_.protection_;

    if (checkCondition && effect == 0 && protection >= 0xFF) {
        protection = 0xFF;  // clamp à 255
    }

    return protection;
}


uint8_t status::HaveStatusInfo::getWisdom(HaveStatusInfo_0* self,int effect) {
    status::HaveStatus* p_haveStatus = &self->haveStatus_;
    uint8_t wisdom = status::HaveStatus::getWisdom(p_haveStatus);

    if ((effect & 1) == 0) {
        status::HaveEquipment::calcEffect(&self->haveEquipment_);

        bool hasEquip148 = status::HaveEquipment::isEquipment(&self->haveEquipment_, 148);
        int baseWisdom = status::HaveStatus::getWisdom(p_haveStatus);

        status::HaveEquipment::calcEffect(&self->haveEquipment_);
        int totalWisdom = self->haveEquipment_.wisdom_ + baseWisdom;

        if (hasEquip148)
            totalWisdom += 15;

        if (totalWisdom >= 0xFF)
            wisdom = 0xFF;
        else
            wisdom = static_cast<uint8_t>(totalWisdom);
    }

    return wisdom;
}


uint8_t status::HaveStatusInfo::getLuck(HaveStatusInfo_0* self, int effect) {
    uint8_t luck = status::HaveStatus::getLuck(&self->haveStatus_);

    if ((effect & 1) == 0) {
        status::HaveEquipment::calcEffect(&self->haveEquipment_);
        int totalLuck = luck + self->haveEquipment_.luck_;

        if (totalLuck >= 0xFF)
            luck = 0xFF;
        else
            luck = static_cast<uint8_t>(totalLuck);
    }

    return luck;
}


void status::HaveStatusInfo::clearAllItem(HaveStatusInfo_0* self)
{
    status::HaveItem::clear(&self->haveItem_);
    status::HaveEquipment::clear(&self->haveEquipment_);
}

status::HaveStatusInfo::HaveStatusInfo() {
    std::memset(this->status_, 0, sizeof(this->status_));

    // Construction des sous-composants
    status::ActionDefence::ActionDefence(this->actionDefence_);
    this->noDamage_ = false;
    this->defenceChange_ = 0;

    this->flag_.setValue(0);
    this->flag2_.setValue(0);
    this->testFlag_.setValue(0);
    this->battleFlag_.setValue(0);

    status::HaveStatus::HaveStatus(this->haveStatus_);
    //status::HaveItem::HaveItem(&this->haveItem_) need to do;
    status::HaveAction::HaveAction(this->haveAction_);
    status::HaveEquipment::HaveEquipment(this->haveEquipment_);
    status::StatusChange::StatusChange(this->statusChange_);
    this->commandTypeDebug_ = dq5::level::CommandTypeDebug::DEFENCE_ONLY;
}

//status::HaveStatusInfo::~HaveStatusInfo()
//{
//    status::StatusChange::~StatusChange(this->statusChange_);
//    status::HaveEquipment::~HaveEquipment(this->haveEquipment_);
//    status::HaveAction::~HaveAction(this->haveAction_);
//    status::HaveItem::~HaveItem(this->haveItem_);
//    status::HaveStatus::~HaveStatus(this->haveStatus_);
//    status::ActionDefence::~ActionDefence(this->actionDefence_);
//}



bool status::HaveStatusInfo::isActionEnable(HaveStatusInfo_0* self)
{
    status::StatusChange* p_statusChange;
    if (status::HaveStatus::getHp(&self->haveStatus_)) {
        p_statusChange = &self->statusChange_;
        if (!status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI) &&
            !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI) &&
            !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_ASTORON) &&
            !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_PATH_1)) {
            return !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DORAGORAM);
        }
    }
    return false;
}

bool status::HaveStatusInfo::isActionEnableWithoutDragoram(HaveStatusInfo_0* self)
{

    status::StatusChange* p_statusChange; 

    if (status::HaveStatus::getHp(&self->haveStatus_))
    {
        p_statusChange = &self->statusChange_;
        if (!status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI)
            && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI)
            && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_ASTORON))
        {
            return !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_PATH_1);
        }
    }
    return false;
}

void status::HaveStatusInfo::setup(HaveStatusInfo_0* self, int index, bool flag)
{
    self->index_ = index;
    self->haveBattleStatus_ = 0;
    self->effectValue_ = 0;
    self->effectValueMax_ = 0;
    self->effectValueMin_ = 0;
    self->flag_.set(0);
    self->flag2_.set(0);
    self->testFlag_.set(0);
    if (flag)
    {
        self->characterType_ = dq5::level::CharacterType::PLAYER;
        status::HaveStatus::setup(&self->haveStatus_, index, 1);
        //status::HaveItem::setup(&self->haveItem_, self->index_);
        status::HaveAction::setup(&self->haveAction_, self->index_, 1);
        status::HaveEquipment::setup(&self->haveEquipment_, &self->haveItem_);
        //status::HaveStatusInfo::setupActionDefence(self);
        status::StatusChange::restore(&self->statusChange_, self->index_);
    }
    else
    {
        self->characterType_ = dq5::level::CharacterType::MONSTER;
        status::HaveStatus::setup(&self->haveStatus_, index, 0);
        //status::HaveItem::setup(&self->haveItem_, 1);
        status::HaveItem::clear(&self->haveItem_);
        status::HaveEquipment::setup(&self->haveEquipment_, &self->haveItem_);
        //status::ActionDefence::setup(&self->actionDefence_, self->index_, dq5::level::CharacterType::MONSTER);
        status::StatusChange::clear(&self->statusChange_);
    }
}

void status::HaveStatusInfo::setAttack(HaveStatusInfo_0* self, uint16_t at)
{
    self->haveStatus_.baseStatus_.strength_ = at;
}

void status::HaveStatusInfo::setDefence(HaveStatusInfo_0* self, uint16_t def)
{
    self->haveStatus_.baseStatus_.protection_ = def;
}


void status::HaveStatusInfo::setSplit(HaveStatusInfo_0* self, bool flag) {
    if (flag)
        self->flag2_.set(16);    
    else
        self->flag2_.clear(16);  
}


bool status::HaveStatusInfo::isSplit(HaveStatusInfo_0* self) {
    return self->flag2_.test(16);
}

void status::HaveStatusInfo::setJouk(HaveStatusInfo_0* self, bool flag) {
    if (flag)
        self->flag2_.set(17);    
    else
        self->flag2_.clear(17);  
}

bool status::HaveStatusInfo::isJouk(HaveStatusInfo_0* self) {
    return self->flag2_.test(17);
}

void status::HaveStatusInfo::setKaishin(HaveStatusInfo_0* self, bool flag) {
    if (flag)
        self->flag2_.set(18);
    else
        self->flag2_.clear(18);
}

void status::HaveStatusInfo::setTsukon1(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(19);
    else
        self->flag2_.clear(19);
}

bool status::HaveStatusInfo::isTsukon1(HaveStatusInfo_0* self)
{
    return self->flag2_.test(19);
}


void status::HaveStatusInfo::setTsukon2(HaveStatusInfo_0* self,bool flag)
{
    if (flag)
        self->flag2_.set(20);
    else
        self->flag2_.clear(20);
}

bool status::HaveStatusInfo::isTsukon2(HaveStatusInfo_0* self)
{
    return self->flag2_.test(20);
}

void status::HaveStatusInfo::setMiss(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(21);  
    else
        self->flag2_.clear(21);  
}

bool status::HaveStatusInfo::isMiss(HaveStatusInfo_0* self)
{
    return self->flag2_.test(21);
}

void status::HaveStatusInfo::setSleepTurn(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(22);  
    else
        self->flag2_.clear(22);  
}

bool status::HaveStatusInfo::isSleepTurn(HaveStatusInfo_0* self)
{
    return self->flag2_.test(22);  

}


void status::HaveStatusInfo::setCurse2(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(23);  
    else
        self->flag2_.clear(23);  
}

bool status::HaveStatusInfo::isCurse2(HaveStatusInfo_0* self)
{
    return self->flag2_.test(23);  
}

void status::HaveStatusInfo::setLifeStoneEnable(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(24);  
    else
        self->flag2_.clear(24);  
}

bool status::HaveStatusInfo::isLifeStoneEnable(HaveStatusInfo_0* self)
{
    return self->flag2_.test(24);
}

void status::HaveStatusInfo::setManusaAlready(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(26);
    else
        self->flag2_.clear(26);
}


bool status::HaveStatusInfo::isManusaAlready(HaveStatusInfo_0* self)
{
    return self->flag2_.test(26);
}

void status::HaveStatusInfo::setCurse1(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(27);
    else
        self->flag2_.clear(27);
}

bool status::HaveStatusInfo::isCurse1(HaveStatusInfo_0* self)
{
    return self->flag2_.test(27);
}

bool status::HaveStatusInfo::isDeathFlag(HaveStatusInfo_0* self)
{
    return self->flag2_.test(20);
}

void status::HaveStatusInfo::setCommandRandom(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(0);
    else
        self->flag_.clear(0);
}

bool status::HaveStatusInfo::isCommandRandom(HaveStatusInfo_0* self)
{
    return self->flag_.test(0);
}

void status::HaveStatusInfo::setZaoraruFailed(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(25);
    else
        self->flag_.clear(25);
}

bool status::HaveStatusInfo::isZaoraruFailed(HaveStatusInfo_0* self)
{
    return self->flag_.test(17);
}

void status::HaveStatusInfo::setMagicDisable(status::HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(30);
    else
        self->flag2_.clear(30);
}
bool status::HaveStatusInfo::isMagicDisable(HaveStatusInfo_0* self)
{
    return self->flag2_.test(14);
}

void status::HaveStatusInfo::setMosyasStart(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(31);
    else
        self->flag2_.clear(31);
}

bool status::HaveStatusInfo::isMosyasStart(status::HaveStatusInfo_0* self)
{
    return self->flag2_.test(31);
}

void status::HaveStatusInfo::setAllKaishin(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(0);
    else
        self->battleFlag_.clear(0);
}

bool status::HaveStatusInfo::isAllKaishin(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(0);
}

void status::HaveStatusInfo::setCloseDoor(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(1);
    else
        self->battleFlag_.clear(1);
}

bool status::HaveStatusInfo::isCloseDoor(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(1);
}

void status::HaveStatusInfo::setMosyasTarget(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(2);  
    else
        self->battleFlag_.clear(2);
}

bool status::HaveStatusInfo::isMosyasTarget(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(2);
}

void status::HaveStatusInfo::setDeathLessly(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(4);  // bit 4 correspond à 0x10
    else
        self->battleFlag_.clear(4);
}

void status::HaveStatusInfo::setMosyasExec(status::HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(3);  // bit 3 correspond à 0x8
    else
        self->battleFlag_.clear(3);
}

bool status::HaveStatusInfo::isMosyasExec(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(3);
}

bool status::HaveStatusInfo::isInvokeMeganteRing(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(5);
}

bool status::HaveStatusInfo::isInvokeRebirthStone(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(10);  
}


void status::HaveStatusInfo::setRebirthForRebirthStone(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(6);  
    else
        self->battleFlag_.clear(6);
}

bool status::HaveStatusInfo::isRebirthForRebirthStone(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(6);
}

void status::HaveStatusInfo::setChangeTactics(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(7);
    else
        self->battleFlag_.clear(7);
}

bool status::HaveStatusInfo::isChangeTactics(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(7);
}

void status::HaveStatusInfo::setEscapeFlag(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->battleFlag_.set(8);
    else
        self->battleFlag_.clear(8);
}


bool status::HaveStatusInfo::isEscapeFlag(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(8);
}

bool status::HaveStatusInfo::isMeganteRing(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(9);
}

void status::HaveStatusInfo::setGlbMeganteRing(bool flag)
{
    if (flag)
        status::HaveStatusInfo::globalFlag_ |= 1; 
    else
        status::HaveStatusInfo::globalFlag_ &= ~1; 
}

int status::HaveStatusInfo::isGlbMeganteRing()
{
    return status::HaveStatusInfo::globalFlag_ & 1;
}

void status::HaveStatusInfo::setGlbMegazaruRing(bool flag)
{
    if (flag)
        status::HaveStatusInfo::globalFlag_ |= 2;   
    else
        status::HaveStatusInfo::globalFlag_ &= ~2;  
}


int status::HaveStatusInfo::isGlbMegazaruRing()
{
    return (status::HaveStatusInfo::globalFlag_ & 2) != 0;
}


void status::HaveStatusInfo::setGlbRebirthStone(bool flag)
{
    if (flag)
        status::HaveStatusInfo::globalFlag_ |= 4;  
    else
        status::HaveStatusInfo::globalFlag_ &= ~4; 
}


int status::HaveStatusInfo::isGlbRebirthStone()
{
    return (status::HaveStatusInfo::globalFlag_ & 4) != 0;
}

void status::HaveStatusInfo::setDrawCtrlId(HaveStatusInfo_0* self, int ctrl)
{
    if (ctrl == -1)
        status::HaveStatus::getHp(&self->haveStatus_);
    self->drawCtrlId_ = ctrl;
}

void status::HaveStatusInfo::releaseMosyas(HaveStatusInfo_0* self)
{
    status::HaveStatus* p_haveStatus; 
    uint16_t Hp;
    uint16_t Mp;

    p_haveStatus = &self->haveStatus_;
    Hp = status::HaveStatus::getHp(&self->haveStatus_);
    Mp = status::HaveStatus::getMp(p_haveStatus);
    status::HaveStatus::setup(p_haveStatus, self->index_, 0);
    status::HaveStatusInfo::setHp(self, Hp);
    if (!self->noDamage_)
        status::HaveStatus::setMp(p_haveStatus, Mp);
}

void status::HaveStatusInfo::execStartOfBattle(HaveStatusInfo_0* self)
{
    self->flag_.reset();
    self->flag2_.reset();
    self->testFlag_.reset();
    status::HaveStatusInfo::clearHpInBattle(self);
    status::HaveStatusInfo::clearMpInBattle(self);
    status::HaveStatusInfo::clearStatusChangeInBattle(self);
    self->battleFlag_.reset();
    status::HaveStatusInfo::globalFlag_ = 0;
    self->specialTargetCount_ = 0;
    self->effectValueMax_ = 0;
    self->effectValueMin_ = 0;
    self->effectValueCount_ = 0;
}

bool status::HaveStatusInfo::isActionEnableForMacro(HaveStatusInfo_0* self)
{
    status::StatusChange* p_statusChange; 
    p_statusChange = &self->statusChange_;
 
    if (!status::StatusChange::isEnable(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI)
        && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI)
        && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_ASTORON))
    {
        return !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_PATH_1);
    }
    return false;
}

bool status::HaveStatusInfo::isAttackEnable(HaveStatusInfo_0* self)
{
    
    status::StatusChange* p_statusChange; 
    if (status::HaveStatus::getHp(&self->haveStatus_))
    {
        p_statusChange = &self->statusChange_;
        if (!status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI)
            && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI)
            && !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_ASTORON))
        {
            return !status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_PATH_1);
        }
    }
    return false;
}

void status::HaveStatusInfo::setupStatus(HaveStatusInfo_0* self, int index, bool flag)
{
    status::HaveStatus* p_haveStatus; 
    char CurrentIndex; 

    self->index_ = index;
    if (flag)
    {
        self->characterType_ = dq5::level::CharacterType::PLAYER;
        p_haveStatus = &self->haveStatus_;
        CurrentIndex = index;
        if (index)
            CurrentIndex = 1;
        status::HaveStatus::setup(p_haveStatus, index, CurrentIndex);
        status::ActionDefence::setup(&self->actionDefence_, self->index_, dq5::level::CharacterType::PLAYER);
    }
    else
    {
        self->characterType_ = dq5::level::CharacterType::MONSTER;
        status::HaveStatus::setup(&self->haveStatus_, index, 0);
        status::ActionDefence::setup(&self->actionDefence_, self->index_, dq5::level::CharacterType::MONSTER);
    }
}