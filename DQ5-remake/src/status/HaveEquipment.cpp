#include "status/HaveEquipment.h"
#include "status/HaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"
#include <iostream>

bool status::HaveEquipment::isEquipment(const HaveEquipment* self, int itemIndex){
        return HaveItem::isEquipmentInAllItem(self->haveItem_, itemIndex);
    }


int status::HaveEquipment::getEquipment(const HaveEquipment* self, dq5::level::ItemType type, int index) {

    return status::HaveItem::getEquipmentItem(self->haveItem_, type, index);
}


void status::HaveEquipment::calcEffect(HaveEquipment* self)
{
    if (!self || !self->haveItem_)
        return;

    // Réinitialise les bonus cumulés
    self->strength_ = 0;
    self->wisdom_ = 0;
    self->attack_ = 0;

    // Arme
    int weapon = status::HaveItem::getEquipmentItem(self->haveItem_, dq5::level::ItemType::ITEM_WEAPON, 0);
    status::HaveEquipment::calcEffect(self, weapon, dq5::level::ItemType::ITEM_WEAPON);

    // Armure
    int armor = status::HaveItem::getEquipmentItem(self->haveItem_, dq5::level::ItemType::ITEM_ARMOR, 0);
    status::HaveEquipment::calcEffect(self, armor, dq5::level::ItemType::ITEM_ARMOR);

    // Bouclier
    int shield = status::HaveItem::getEquipmentItem(self->haveItem_, dq5::level::ItemType::ITEM_SHIELD, 0);
    status::HaveEquipment::calcEffect(self, shield, dq5::level::ItemType::ITEM_SHIELD);

    // Casque
    int helmet = status::HaveItem::getEquipmentItem(self->haveItem_, dq5::level::ItemType::ITEM_HELMET, 0);
    status::HaveEquipment::calcEffect(self, helmet, dq5::level::ItemType::ITEM_HELMET);

    // Accessoires (7 emplacements max)
    for (int i = 0; i < 7; ++i)
    {
        int accessory = status::HaveItem::getEquipmentItem(self->haveItem_, dq5::level::ItemType::ITEM_ACCESSORY, i);
        status::HaveEquipment::calcEffect(self, accessory, dq5::level::ItemType::ITEM_ACCESSORY);
    }
}


void status::HaveEquipment::calcEffect(status::HaveEquipment* self, int itemIndex, dq5::level::ItemType type)
{
    if (!itemIndex)
        return;

    // Cas où un équipement est explicitement ignoré (exclusion temporaire ?)
    if (self->calcNoEquipmentItemIndex_ && self->calcNoEquipmentItemIndex_ == itemIndex)
    {
        self->calcNoEquipmentItemIndex_ = 0;
        return;
    }

    int calcItem = itemIndex;

    // Si un équipement est déjà en attente de traitement (et match le type)
    if (self->calcEquipmentItemIndex_ != 0)
    {
        dq5::level::ItemType existingType = status::UseItem::getEquipItemType(self->calcEquipmentItemIndex_);
        dq5::level::ItemType newType = status::UseItem::getEquipItemType(itemIndex);

        if (existingType == newType)
        {
            // On ignore le nouvel item et utilise l'ancien
            calcItem = self->calcEquipmentItemIndex_;
            self->calcEquipmentItemIndex_ = 0;
        }
        else if (status::UseItem::getEquipItemType(self->calcEquipmentItemIndex_) != type)
        {
            // Type incompatible : on ne traite rien
            return;
        }
        else
        {
            calcItem = self->calcEquipmentItemIndex_;
            self->calcEquipmentItemIndex_ = 0;
        }
    }

    if (!calcItem)
        return;

    // Type d'effet (attaque, défense, agilité...)
    int effectType = status::UseItem::getEquipType(calcItem);
    int value = status::UseItem::getEquipValue(calcItem);

    switch (effectType)
    {
    case 0: // ATK
        self->attack_ += value;
        break;
    case 1: // DEF
        self->defence_ += value;
        break;
    case 2: // STR
        self->strength_ += value;
        break;
    case 3: // AGI (sauf cas 10000 magique)
        if (std::abs(value) != 10000)
            self->agility_ += value;
        break;
    case 4: // LUCK
        self->luck_ += value;
        break;
    case 5: // WIS
        self->wisdom_ += value;
        break;
    default:
        return;
    }
}

void status::HaveEquipment::resetEquipment(HaveEquipment* self, int itemIndex)
{
   
}

void status::HaveEquipment::clear(HaveEquipment* self) {
    self->luck_ = 0;
    self->strength_ = 0;
    self->attack_ = 0;
}


void status::HaveEquipment::setup(HaveEquipment* self, HaveItem* haveItem)
{
    self->haveItem_ = haveItem;
    self->calcNoEquipmentItemIndex_ = 0;
    self->calcEquipmentItemIndex_ = 0;
}

void status::HaveEquipment::setup(HaveEquipment* self, int index)
{
    self->luck_ = 0;
    *&self->strength_ = 0;
    *&self->attack_ = 0;
    self->playerIndex_ = index;
}