#include "ar/BitFlag.h"
#include "status/PlayerData.h"
#include "dq5/CharacterInitData.h"


bool status::PlayerData::isCurse4(PlayerData* self) {
    return self->flag_.test(0); 
}

bool status::PlayerData::isCurse5(PlayerData* self) {
    return self->flag_.test(1); // bit 1
}

bool status::PlayerData::isPoison(PlayerData* self)
{
    return self->flag_.test(2);
}

bool status::PlayerData::isVenom(PlayerData* self)
{
    return self->flag_.test(3);
}


void status::PlayerData::setCurse4(PlayerData* self, bool flag) {
    if (flag)
        self->flag_.set(0);   
    else
        self->flag_.clear(0); 
}

void status::PlayerData::setCurse5(PlayerData* self, bool flag) {
    if (flag)
        self->flag_.set(1);  
    else
        self->flag_.clear(1); 
}


void status::PlayerData::setPoison(PlayerData* self, bool flag) {
    if (flag)
        self->flag_.set(2);   
    else
        self->flag_.clear(2); 
}


void status::PlayerData::setVenom(PlayerData* self, bool flag) {
    if (flag)
        self->flag_.set(3);  
    else
        self->flag_.clear(3);
}


int status::PlayerData::getCharacterKind(status::PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );

    uint8_t byte = *(reinterpret_cast<uint8_t*>(record) + 0x54);

 
    return (byte >> 3) & 0x7;
}

uint16_t status::PlayerData::getActionMonsterIndex(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );

    uint16_t byte = *(reinterpret_cast<uint16_t*>(record) + 0x1B);
    return 0;
}

int status::PlayerData::getAttack(PlayerData* self)
{
    // Récupérer la force de base du personnage
    int strength = self->baseStatus_.strength_;

    // Appliquer les effets d’équipement (modifie self->haveEquipment_.attack_, etc.)
    status::HaveEquipment::calcEffect(&self->haveEquipment_);

    // Calcul de l'attaque totale
    int totalAttack = self->haveEquipment_.attack_ + strength;

    // Cas spécial : équipement avec ID 66 force la valeur d’attaque à 69
    if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 66))
        return 69;

    return totalAttack;
}
