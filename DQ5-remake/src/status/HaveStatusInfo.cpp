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


bool status::HaveStatusInfo::isMultiAttack(HaveStatusInfo_0* self)
{
    return self->flag_.test(10);
}


void status::HaveStatusInfo::setMultiAttack(HaveStatusInfo_0* self, bool flag) {

    if (flag)
        self->flag_.set(10);
    else
        self->flag_.clear(10);
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
        status::HaveStatusInfo::execDeath(self);
    }
}


void status::HaveStatusInfo::setDeath(HaveStatusInfo_0* self) {
    status::HaveStatusInfo::setHp(self, 0);
}


int status::HaveStatusInfo::setItemEquipment(HaveStatusInfo_0* self,int itemIndex) {
    if (itemIndex == 0)
        return -1;

    int itemPos = self->haveItem_.VBaseHaveItemAdd(itemIndex);
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

void status::HaveStatusInfo::resetEquipment(HaveStatusInfo_0* self, dq5::level::ItemType itemType)
{
    status::HaveEquipment* p_haveEquipment; 
    int Equipment; 

    p_haveEquipment = &self->haveEquipment_;
    Equipment = status::HaveEquipment::getEquipment(&self->haveEquipment_, itemType, 0);
    status::HaveEquipment::resetEquipment(p_haveEquipment, Equipment);
    status::BaseHaveItem::resetEquipmentWithItemIndex(&self->haveItem_, Equipment);
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

        status::HaveStatusInfo::execDeath(self);
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
    self->haveItem_.VBaseHaveItemClear();
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
    this->haveItem_ = status::HaveItem();
    status::HaveAction::HaveAction(this->haveAction_);
    status::HaveEquipment::HaveEquipment(this->haveEquipment_);
    status::StatusChange::StatusChange(this->statusChange_);
    this->commandTypeDebug_ = dq5::level::CommandTypeDebug::DEFENCE_ONLY;
}

status::HaveStatusInfo::~HaveStatusInfo()
{
    this->statusChange_.~StatusChange();
    this->haveEquipment_.~HaveEquipment();
    this->haveAction_.~HaveAction();
    this->haveItem_.~HaveItem();
    this->haveStatus_.~HaveStatus();
    this->actionDefence_.~ActionDefence();
}



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
        self->haveItem_.VBaseHaveItemClear();
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
    return (status::HaveStatusInfo::globalFlag_ >> 1) & 1;
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
    return (status::HaveStatusInfo::globalFlag_ >> 2) & 1;
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

void status::HaveStatusInfo::setExp(HaveStatusInfo_0* self, int exp)
{
    status::HaveStatus::setExp(&self->haveStatus_, exp);
}

void status::HaveStatusInfo::setLevelupExp(HaveStatusInfo_0* self, int val)
{
    status::HaveStatus::setLevelupExp(&self->haveStatus_, val);
}

void status::HaveStatusInfo::setLuck(HaveStatusInfo_0* self, uint8_t  luck)
{
   self->haveStatus_.baseStatus_.luck_ = luck;
}

void status::HaveStatusInfo::rebirth(HaveStatusInfo_0* self)
{
    dq5::level::CharacterType characterType;
  
    characterType = self->characterType_;
    uint32_t HpMax = status::HaveStatus::getHpMax(&self->haveStatus_);
    uint16_t MaxHp = 999;
    if (HpMax < 0x3E7)
        MaxHp = HpMax;
    if (characterType != dq5::level::CharacterType::NONE)
        MaxHp = HpMax;
    status::HaveStatusInfo::setHp(self, MaxHp);
}


void status::HaveStatusInfo::setActionDisable(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(8);
    else
        self->flag_.clear(8);
}

void status::HaveStatusInfo::setAddEffectDamage(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(21);
    else
        self->flag_.clear(21);
}

void status::HaveStatusInfo::setAddEffectPoison(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(19);
    else
        self->flag_.clear(19);
}


void status::HaveStatusInfo::setAddEffectRecovery(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(20);
    else
        self->flag_.clear(20);
}

void status::HaveStatusInfo::setAddEffectSleep(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(17);
    else
        self->flag_.clear(17);
}



void status::HaveStatusInfo::setAddEffectSpazz(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(18);
    else
        self->flag_.clear(18);
}


void status::HaveStatusInfo::setAddMahotoraExecute(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(31);
    else
        self->flag_.clear(31);
}

void status::HaveStatusInfo::setAgility(HaveStatusInfo_0* self, uint8_t agi)
{
    self->haveStatus_.baseStatus_.agility_ = agi;
}

void status::HaveStatusInfo::setAstoron(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(4);
    else
        self->flag2_.clear(4);
}

uint16_t status::HaveStatusInfo::setAttackChange(HaveStatusInfo_0* self)
{
    uint16_t result; 

    uint16_t BaseAttack = status::HaveStatusInfo::getBaseAttack(self);
    uint16_t AtkMax = 0;
    if (status::HaveStatusInfo::getAttack(self, 0) != 9999)
    {
        AtkMax = BaseAttack;
        if (status::HaveStatusInfo::getAttack(self, 0) + BaseAttack >= 10000)
            AtkMax = 9999 - status::HaveStatusInfo::getAttack(self, 0);
    }
    result = AtkMax;
    if (!self->attackChange_)
        self->attackChange_ = AtkMax;
    return result;
}


uint16_t status::HaveStatusInfo::getBaseAttack(HaveStatusInfo_0* self) {
    HaveEquipment* p_haveEquipment = &self->haveEquipment_;
    bool isEquipment = status::HaveEquipment::isEquipment(p_haveEquipment, 66);
    uint16_t fixedValue = 69;

    if (isEquipment)
        return fixedValue;

    uint16_t strength = self->haveStatus_.baseStatus_.strength_;

    status::HaveEquipment::calcEffect(p_haveEquipment);

    uint16_t totalStrength = static_cast<uint16_t>(strength + self->haveEquipment_.strength_);

    if (self->characterType_ == dq5::level::CharacterType::PLAYER) {
        if (totalStrength >= 0xFF) {
            totalStrength = 0xFF;
        }
        totalStrength = static_cast<uint8_t>(totalStrength);
    }

    uint16_t totalAttack = static_cast<uint16_t>(self->haveEquipment_.attack_ + totalStrength);

    if (totalAttack == 0)
        return 0;

    if (totalAttack <= 9998)
        return totalAttack;

    return 9999;
}

void status::HaveStatusInfo::addExp(HaveStatusInfo_0* self, int exp)
{
    status::HaveStatus::addExp(&self->haveStatus_, exp);
}


void status::HaveStatusInfo::addHpInBattle(HaveStatusInfo_0* self,HaveStatusInfo::DiffStatus status,int hp)
{
    status::HaveStatusInfo::addHp(self, hp);
}

void status::HaveStatusInfo::addHpMax(HaveStatusInfo_0* self, uint16_t hp)
{
    status::HaveStatus* p_haveStatus; 

    p_haveStatus = &self->haveStatus_;
    status::HaveStatus::addHpMax(&self->haveStatus_, hp);
    if (self->characterType_ == dq5::level::CharacterType::PLAYER && status::HaveStatus::getHpMax(p_haveStatus) >= 1000)
        status::HaveStatus::setHpMax(p_haveStatus, 999);
}

void status::HaveStatusInfo::addLuck(HaveStatusInfo_0* self, char luck)
{
    status::HaveStatus::addLuck(&self->haveStatus_, luck);
}

void status::HaveStatusInfo::addMp(HaveStatusInfo_0* self, int mp)
{
    if (!self->noDamage_)
        status::HaveStatus::addBaseMp(&self->haveStatus_, mp);
}

void status::HaveStatusInfo::addMpInBattle(HaveStatusInfo_0* self,HaveStatusInfo::DiffStatus status,int mp)
{
    if (!self->noDamage_)
        status::HaveStatus::addBaseMp(&self->haveStatus_, mp);
}

void  status::HaveStatusInfo::addMpMax(HaveStatusInfo_0* self, uint16_t mp)
{
    status::HaveStatus::addMpMax(&self->haveStatus_, mp);
}

void  status::HaveStatusInfo::addProtection(HaveStatusInfo_0* self, int pro)
{
    status::HaveStatus::addProtection(&self->haveStatus_, pro);
}

void  status::HaveStatusInfo::addSpecialTargetCount(HaveStatusInfo_0* self)
{
    ++self->specialTargetCount_;
}

void status::HaveStatusInfo::addStrength(HaveStatusInfo_0* self, int str)
{
    status::HaveStatus::addStrength(&self->haveStatus_, str);
}


void status::HaveStatusInfo::addWisdom(HaveStatusInfo_0* self, char wis)
{
    status::HaveStatus::addWisdom(&self->haveStatus_, wis);
}

void status::HaveStatusInfo::breakSpell(HaveStatusInfo_0* self)
{
    status::StatusChange* p_statusChange; 

    p_statusChange = &self->statusChange_;
    if (status::StatusChange::isEnable(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE1))
        status::StatusChange::cleanup(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE1);
    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE2))
        status::StatusChange::cleanup(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE2);
    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE3))
        status::StatusChange::cleanup(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE3);
    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4))
        status::StatusChange::cleanup(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4);
    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5))
        status::StatusChange::cleanup(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5);
}


void status::HaveStatusInfo::cleanup(HaveStatusInfo_0* self)
{
    if (self->characterType_ == dq5::level::CharacterType::PLAYER)
    {
        status::HaveStatus::cleanup(&self->haveStatus_);
        status::HaveItem::cleanup(&self->haveItem_, self->index_);
        status::HaveAction::cleanup(&self->haveAction_, self->index_);
        status::StatusChange::store(&self->statusChange_, self->index_);
    }
}


void status::HaveStatusInfo::clearAgilityChange(HaveStatusInfo_0* self)
{
    self->agilityChange_ = 0;
}

void status::HaveStatusInfo::clearAttackChange(HaveStatusInfo_0* self)
{
    self->attackChange_ = 0;
}

void status::HaveStatusInfo::clearDefenceChange(HaveStatusInfo_0* self)
{
    self->defenceChange_ = 0;
}


void status::HaveStatusInfo::detoxPoison(HaveStatusInfo_0* self)
{
    status::StatusChange* p_statusChange; 

    p_statusChange = &self->statusChange_;
    if (status::StatusChange::isEnable(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU))
        status::StatusChange::release(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU);
}


int16_t status::HaveStatusInfo::getAgilityChange(HaveStatusInfo_0* self)
{
    bool isEnable;

    isEnable = status::StatusChange::isEnable(
        &self->statusChange_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SPD_PLUS);
   
    if (isEnable)
        return self->agilityChange_;
    return 0;
}

void status::HaveStatusInfo::setMpFailure(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(5);
    else
        self->flag2_.clear(5);
}



void status::HaveStatusInfo::setMahotoneFailure(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(6);
    else
        self->flag2_.clear(6);
}

void status::HaveStatusInfo::setBaikirutoDisable(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(25);
    else
        self->flag_.clear(25);
}

void status::HaveStatusInfo::setImmidiateDeath(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag2_.set(10);
    else
        self->flag2_.clear(10);
}

void status::HaveStatusInfo::setWeaponAddDamage(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(14);
    else
        self->flag_.clear(14);
}

void status::HaveStatusInfo::setDamageMyself(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(3);
    else
        self->flag_.clear(3);
}

void status::HaveStatusInfo::setCounterDamage(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(28);
    else
        self->flag_.clear(28);
}

bool status::HaveStatusInfo::isTargetJoukForTest(HaveStatusInfo_0* self)
{
    return self->testFlag_.test(0);;
}

void status::HaveStatusInfo::setKillMyself(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(16);
    else
        self->flag_.clear(16);
}


bool status::HaveStatusInfo::isKillMyself(HaveStatusInfo_0* self)
{
    return self->flag_.test(16);
}


void status::HaveStatusInfo::setMonsterChange(HaveStatusInfo_0* self, bool flag)
{
    if (flag)
        self->flag_.set(24);
    else
        self->flag_.clear(24);
}


void status::HaveStatusInfo::levelup(HaveStatusInfo_0* self, int level)
{
    if (level != 0)
    {
        int levelMax = level;
        if (self->haveStatus_.levelMax_ < level)
            levelMax = self->haveStatus_.levelMax_;

        status::HaveStatus::debugLevelup(&self->haveStatus_, levelMax);
        status::HaveAction::debugLevelup(&self->haveAction_, levelMax);
    }
    else
    {
        status::HaveStatus::levelup(&self->haveStatus_, 0);
        status::HaveAction::levelup(&self->haveAction_, self->index_, self->haveStatus_.level_, true);
    }
}

void status::HaveStatusInfo::execDeath(status::HaveStatusInfo_0* self)
{
    status::HaveBattleStatus* haveBattleStatus; 

    if (!status::HaveStatus::getHp(&self->haveStatus_))
    {
        status::StatusChange::release(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NEMURI);
        status::StatusChange::release(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_KONRAN);
        status::StatusChange::release(&self->statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHOKANTA);
        haveBattleStatus = self->haveBattleStatus_;
        if (haveBattleStatus)
            status::HaveBattleStatus::changeMonsterReverse(haveBattleStatus);
    }
}


void status::HaveStatusInfo::setupActionDefence(HaveStatusInfo_0* self, int index, bool flag)
{
    status::ActionDefence::setup(&self->actionDefence_, index, (dq5::level::CharacterType) !flag);
}

int status::HaveStatusInfo::getLevelupExp(const HaveStatusInfo_0* self)
{
    return status::HaveStatus::getLevelupExp(&self->haveStatus_);
}


int status::HaveStatusInfo::getIconIndex(HaveStatusInfo_0* self)
{
  
    int playerIndex = self->haveStatus_.playerIndex_;
    if (playerIndex == 7)
    {
        if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 69))
            return 95;
        playerIndex = self->haveStatus_.playerIndex_;
    }
    if (playerIndex == 14)
        return status::HaveStatusInfo::getDaughterIconIndex();
    if (playerIndex == 13)
        return status::HaveStatusInfo::getSonIconIndex();
    return status::HaveStatus::getIconIndex(&self->haveStatus_);
}


int status::HaveStatusInfo::getMpInBattle(HaveStatusInfo_0* self, status::HaveStatusInfo::DiffStatus status)
{
    return self->mp_[static_cast<int>(status)];
}


uint16_t status::HaveStatusInfo::getMpMax(const HaveStatusInfo_0* self)
{
    dq5::level::CharacterType characterType; 
    unsigned int MpMax = status::HaveStatus::getMpMax(&self->haveStatus_);
    int16_t MaxAllowedMP = 999;
    characterType = self->characterType_;

    if (MpMax < 999)
        MaxAllowedMP = MpMax;
    if (characterType == dq5::level::CharacterType::PLAYER)
        MpMax = MaxAllowedMP;
    return MpMax;
}


int status::HaveStatusInfo::getSpecialTargetCount(HaveStatusInfo_0* self)
{
    return self->specialTargetCount_;
}

int status::HaveStatusInfo::getSpecialTargetDamage(HaveStatusInfo_0* self)
{
    return self->specialTargetDamage_;
}

bool status::HaveStatusInfo::isAddEffectDamage(HaveStatusInfo_0* self)
{
    return self->flag_.test(22);
}


bool status::HaveStatusInfo::isAddEffectMahotora(HaveStatusInfo_0* self)
{
    return self->flag_.test(21);
}

bool status::HaveStatusInfo::isAddEffectPoison(HaveStatusInfo_0* self)
{
    return self->flag_.test(19);
}

bool status::HaveStatusInfo::isAddEffectRecovery(HaveStatusInfo_0* self)
{
    return self->flag_.test(20);
}

bool status::HaveStatusInfo::isAddEffectSleep(HaveStatusInfo_0* self)
{
    return self->flag_.test(17);
}

bool status::HaveStatusInfo::isAddEffectSpazz(HaveStatusInfo_0* self)
{
    return self->flag_.test(18);
}

bool status::HaveStatusInfo::isAddMahotoraExecute(HaveStatusInfo_0* self)
{
    return self->flag_.test(31);
}

bool status::HaveStatusInfo::isAstoron(HaveStatusInfo_0* self)
{
    return self->flag2_.test(4);
}

bool status::HaveStatusInfo::isBaikirutoDisable(HaveStatusInfo_0* self)
{
    return self->flag_.test(9);
}

bool status::HaveStatusInfo::isBossDeathFlag(HaveStatusInfo_0* self)
{
    return self->flag_.test(2);
}

bool status::HaveStatusInfo::isCallFriend(HaveStatusInfo_0* self)
{
    return self->flag_.test(12);
}

bool status::HaveStatusInfo::isCommandRandom(HaveStatusInfo_0* self)
{
    return self->flag_.test(0);
}

void status::HaveStatusInfo::setupActionDefence(HaveStatusInfo_0* self)
{
    status::ActionDefence* p_actionDefence = &self->actionDefence_;
    status::ActionDefence::setup(&self->actionDefence_, self->index_, dq5::level::CharacterType::PLAYER);
    int playerType = static_cast<uint8_t>(self->haveStatus_.playerType_);
    if (playerType == 5)
    {
        uint16_t monsterId = self->haveStatus_.monsterId_;
        status::ActionDefence::setup(p_actionDefence, monsterId, dq5::level::CharacterType::MONSTER);
        uint16_t playerKindIndex = self->haveStatus_.playerKindIndex_;
        self->actionDefence_.nifram_ = 3;
        if ((playerKindIndex & 0xFFFE) == 0x3A)
            status::ActionDefence::setup(p_actionDefence, monsterId, dq5::level::CharacterType::PLAYER);
    }
    else if (playerType == 3)
    {
        status::ActionDefence::setup(p_actionDefence, self->haveStatus_.monsterIndex_, dq5::level::CharacterType::MONSTER);
    }
}

bool status::HaveStatusInfo::isConfuseMissAttack(HaveStatusInfo_0* self)
{
    return self->flag_.test(15);
}


bool status::HaveStatusInfo::isCounterDamage(status::HaveStatusInfo_0* self)
{
    return self->flag_.test(18);
}


status::HaveStatusInfo::Condition status::HaveStatusInfo::getCondition(status::HaveStatusInfo_0* self)
{
    if (status::HaveStatus::getHp(&self->haveStatus_) == 0)
        return status::HaveStatusInfo::Condition::DEAD; 

    status::StatusChange* p_statusChange = &self->statusChange_;

    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU))
        return status::HaveStatusInfo::Condition::POISON; 

    if (status::StatusChange::isEnable(p_statusChange, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAHI))
        return status::HaveStatusInfo::Condition::SPAZZ; 
    
    return status::HaveStatusInfo::Condition::POISON;
}



bool status::HaveStatusInfo::isDisableTextureCache(HaveStatusInfo_0* self)
{
    return self->flag2_.test(15);  
}


bool status::HaveStatusInfo::isDeathLessly(HaveStatusInfo_0* self)
{
    return self->battleFlag_.test(4);
}


bool status::HaveStatusInfo::isDisappearFlag(HaveStatusInfo_0* self)
{
    return self->flag_.test(5);
}


bool status::HaveStatusInfo::isEquipEnable(const HaveStatusInfo_0* self, int itemIndex)
{
    return status::UseItem::isEquipEnable(self->haveStatus_.equipAttrIndex_, itemIndex, 0);
}

bool status::HaveStatusInfo::isEvilPriest(HaveStatusInfo_0* self)
{
    return self->flag_.test(7);
}


bool status::HaveStatusInfo::isExecuteMeganteRing(HaveStatusInfo_0* self)
{
    return self->flag2_.test(12); 
}


bool status::HaveStatusInfo::isFirstKaishin(HaveStatusInfo_0* self)
{
    return self->flag2_.test(1);
}


bool status::HaveStatusInfo::isFirstMosyas(HaveStatusInfo_0* self)
{
    return self->flag_.test(10);
}


bool status::HaveStatusInfo::isFubahaFlag(HaveStatusInfo_0* self)
{
    return self->flag_.test(1);
}


bool status::HaveStatusInfo::isPlayer(const HaveStatusInfo_0* self, int playerIndex)
{
    uint8_t playerType = static_cast<uint8_t>(self->haveStatus_.playerType_);

    bool isValidType = playerType <= 5 && ((1 << playerType) & 0x26);

    return isValidType && self->haveStatus_.playerIndex_ == playerIndex;
}


bool status::HaveStatusInfo::isNeedRecovery(HaveStatusInfo_0* self)
{
    status::HaveStatus* p_haveStatus; 

    p_haveStatus = &self->haveStatus_;
    int Hp = status::HaveStatus::getHp(&self->haveStatus_);
    int HpMax = status::HaveStatus::getHpMax(p_haveStatus);
    if (Hp == HpMax)
    {
        int Mp = status::HaveStatus::getMp(p_haveStatus);
        return Mp != status::HaveStatus::getMpMax(p_haveStatus);
    }
    return 1;
}

bool status::HaveStatusInfo::isMpFailure(HaveStatusInfo_0* self)
{
    return self->flag2_.test(5);
}


bool status::HaveStatusInfo::isMultiFirstDeath(HaveStatusInfo_0* self)
{
    return self->flag2_.test(11);
}


bool status::HaveStatusInfo::isNearDeath(const HaveStatusInfo_0* self)
{
    const status::HaveStatus* p_haveStatus = &self->haveStatus_;

    if (!status::HaveStatus::getHp(p_haveStatus))
        return false;

    int16_t Hp = status::HaveStatus::getHp(p_haveStatus);
    dq5::level::CharacterType characterType = self->characterType_;
    uint32_t HpMax = status::HaveStatus::getHpMax(p_haveStatus);

    uint32_t CapHp = 999;
    if (HpMax < 999)
        CapHp = HpMax;

    if (characterType == dq5::level::CharacterType::NONE)
        CapHp = HpMax;

    return Hp < static_cast<int>(CapHp >> 2);
}


uint16_t status::HaveStatusInfo::getAttackChange(HaveStatusInfo_0* self)
{
    return 0;
}


uint8_t status::HaveStatusInfo::getBaseAgility(HaveStatusInfo_0* self)
{
    status::HaveStatus* p_haveStatus; 
    int EquipementAgility;
    int BoostAgilityEquipement;
    unsigned int TotalAgility;
    uint8_t result;

    p_haveStatus = &self->haveStatus_;
    status::HaveStatus::getAgility(&self->haveStatus_);
    status::HaveEquipment::calcEffect(&self->haveEquipment_);
    if (status::HaveEquipment::getEquipment(&self->haveEquipment_, dq5::level::ItemType::ITEM_ARMOR, 0) == 70)
        return 0;
    int Equipment = status::HaveEquipment::getEquipment(&self->haveEquipment_, dq5::level::ItemType::ITEM_ACCESSORY, 0);
    int Agility = status::HaveStatus::getAgility(p_haveStatus);

    status::HaveEquipment::calcEffect(&self->haveEquipment_);

    if (Equipment == 155)
    {
        EquipementAgility = self->haveEquipment_.agility_;
        BoostAgilityEquipement = 2 * Agility;
    }
    else
    {
        BoostAgilityEquipement = self->haveEquipment_.agility_;
        EquipementAgility = Agility;
    }
    TotalAgility = BoostAgilityEquipement + EquipementAgility;
    if (!(BoostAgilityEquipement + EquipementAgility))
        return 0;
    result = -1;
    if (TotalAgility <= 0xFE)
        return TotalAgility;
    return result;
}

uint16_t status::HaveStatusInfo::getBaseDefence(HaveStatusInfo_0* self)
{
    status::HaveEquipment* p_haveEquipment = &self->haveEquipment_;
    dq5::level::CharacterType characterType = self->characterType_;
    uint16_t protection = self->haveStatus_.baseStatus_.protection_;

    status::HaveEquipment::calcEffect(p_haveEquipment);

    int16_t currentProtection = protection;
    if (protection >= 0xFF)
        currentProtection = 255;
    if (characterType != dq5::level::CharacterType::NONE)
        currentProtection = protection;

    int16_t totalProtection = currentProtection + self->haveEquipment_.defence_;

    if (status::HaveEquipment::isEquipment(p_haveEquipment, 117) ||
        status::HaveEquipment::isEquipment(p_haveEquipment, 23))
    {
        totalProtection = 0;
    }
    else if (status::HaveEquipment::isEquipment(p_haveEquipment, 66))
    {
        totalProtection = 54;
    }

    if (totalProtection <= 0)
        totalProtection = 0;
    else if (totalProtection >= 9999)
        return 9999;

    return static_cast<uint16_t>(totalProtection);
}


uint8_t status::HaveStatusInfo::getBeforeAgility(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    int Agility; 
    int EquipmentAgility;
    uint8_t result; 
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    Agility = status::HaveStatus::getAgility(&self->haveStatus_);


    status::HaveEquipment::calcEffect(&HaveEquipment);
    EquipmentAgility = HaveEquipment.agility_;
    HaveEquipment.~HaveEquipment();
    result = EquipmentAgility + Agility;
    if (static_cast<unsigned int>(result) >= 0xFF)
        return -1;
    return result;
}


int16_t status::HaveStatusInfo::getBeforeAttack(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    int strength; 
    int attack; 
    int16_t MaxAttack;
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    strength = self->haveStatus_.baseStatus_.strength_;
    status::HaveEquipment::calcEffect(&HaveEquipment);
    attack = HaveEquipment.attack_;
    HaveEquipment.~HaveEquipment();
    MaxAttack = 9999;
    if (static_cast<unsigned int>((attack + strength)) < 9999)
        return attack + strength;
    return MaxAttack;
}


int16_t status::HaveStatusInfo::getBeforeDefence(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    int protection; 
    int defence; 
    short MaxDef; 
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    protection = self->haveStatus_.baseStatus_.protection_;
    status::HaveEquipment::calcEffect(&HaveEquipment);
    defence = HaveEquipment.defence_;
    HaveEquipment.~HaveEquipment();
    MaxDef = 9999;
    if (static_cast<unsigned int>(defence + protection) < 9999)
        return defence + protection;
    return MaxDef;
}

uint8_t status::HaveStatusInfo::getBeforeLuck(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    int Luck; 
    int EquipmentLuck;
    uint8_t result;
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    Luck = status::HaveStatus::getLuck(&self->haveStatus_);
    status::HaveEquipment::calcEffect(&HaveEquipment);
    EquipmentLuck = HaveEquipment.luck_;
    HaveEquipment.~HaveEquipment();
    result = EquipmentLuck + Luck;
    if (static_cast<unsigned int>(EquipmentLuck + Luck) >= 0xFF)
        return -1;
    return result;
}

uint8_t status::HaveStatusInfo::getBeforeStrength(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    uint16_t strength; 
    uint8_t result; 
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    strength = self->haveStatus_.baseStatus_.strength_;
    status::HaveEquipment::calcEffect(&HaveEquipment);
    EquipmentStrength = HaveEquipment.strength_;
    HaveEquipment.~HaveEquipment();
    result = strength + EquipmentStrength;
    if (static_cast<uint16_t>(strength + EquipmentStrength) >= 0xFF)
        return -1;
    return result;
}


uint8_t status::HaveStatusInfo::getBeforeWisdom(const HaveStatusInfo_0* self, int itemIndex)
{
    int calcNoEquipmentItemIndex; 
    int calcEquipmentItemIndex; 
    int EquipmentStrength;
    int EquipmentMp;
    int EquipmentDefence;
    status::HaveItem* haveItem; 
    int Wisdom; 
    int EquipmentWisdom;
    uint8_t result; 
    status::HaveEquipment HaveEquipment;

    calcNoEquipmentItemIndex = self->haveEquipment_.calcNoEquipmentItemIndex_;
    calcEquipmentItemIndex = self->haveEquipment_.calcEquipmentItemIndex_;
    EquipmentStrength = self->haveEquipment_.strength_;
    HaveEquipment.playerIndex_ = self->haveEquipment_.playerIndex_;
    HaveEquipment.calcNoEquipmentItemIndex_ = calcNoEquipmentItemIndex;
    HaveEquipment.calcEquipmentItemIndex_ = calcEquipmentItemIndex;
    HaveEquipment.strength_ = EquipmentStrength;
    EquipmentMp = self->haveEquipment_.mp_;
    EquipmentDefence = self->haveEquipment_.defence_;
    haveItem = self->haveEquipment_.haveItem_;
    HaveEquipment.luck_ = self->haveEquipment_.luck_;
    HaveEquipment.mp_ = EquipmentMp;
    HaveEquipment.defence_ = EquipmentDefence;
    HaveEquipment.haveItem_ = haveItem;
    HaveEquipment.calcNoEquipmentItemIndex_ = itemIndex;
    Wisdom = status::HaveStatus::getWisdom(&self->haveStatus_);
    status::HaveEquipment::calcEffect(&HaveEquipment);
    EquipmentWisdom = HaveEquipment.wisdom_;
    HaveEquipment.~HaveEquipment();
    result = EquipmentWisdom + Wisdom;
    if (static_cast<unsigned int>(EquipmentWisdom + Wisdom) >= 0xFF)
        return -1;
    return result;
}