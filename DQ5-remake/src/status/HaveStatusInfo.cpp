#include "status/HaveStatusInfo.h"
#include "status/HaveStatusInfo_0.h"
#include "status/HaveStatus.h"
#include "dq5/ActionParam.h"
#include "ar/BitFlag.h"
#include "status/HaveEquipment.h"
#include "status/UseItem.h"
#include "status/BaseStatus.h"
#include <cstdint>
#include <cstring> // pour std::memset

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

void status::HaveStatusInfo::setImmediateDeathItem(HaveStatusInfo_0* self, bool flag) {
    constexpr uint32_t ImmediateDeathBit = 0x20000000;

    uint32_t current = self->flag2_.get();
    if (flag) {
        self->flag2_.setValue(current | ImmediateDeathBit);  // Active le bit
    }
    else {
        self->flag2_.setValue(current & ~ImmediateDeathBit); // Le désactive
    }
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



uint16_t status::HaveStatusInfo::getDefence(status::HaveStatusInfo_0* self, int effect)
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


bool status::HaveStatusInfo::isKaishin(HaveStatusInfo_0* self) {
    uint8_t byte2 = static_cast<uint8_t>((self->flag2_.get() >> 16) & 0xFF);
    return (byte2 >> 2) & 0x1;
}


bool status::HaveStatusInfo::isTsukon1(HaveStatusInfo_0* self) {
    return ((self->flag2_.get() >> 19) & 0x1);
}


bool status::HaveStatusInfo::isTsukon2(HaveStatusInfo_0* self) {
    return ((self->flag2_.get() >> 20) & 0x1);
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


uint16_t status::HaveStatusInfo::getMp(const status::HaveStatusInfo_0* self) {
    return status::HaveStatus::getMp(&self->haveStatus_);
}



void status::HaveStatusInfo::HaveStatusinfo(HaveStatusInfo_0* self) {
    std::memset(self->status_, 0, sizeof(self->status_));

    status::ActionDefence::initActionDefence(&self->actionDefence_);
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


