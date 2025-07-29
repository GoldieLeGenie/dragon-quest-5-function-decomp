#include "ar/BitFlag.h"
#include "status/PlayerData.h"
#include "status/HaveStatusInfo.h"
#include "status/GameFlag.h"
#include "dq5/CharacterInitData.h"
#include <cstring>
#include <string>

status::PlayerData status::playerData_[232] = {};
uint8_t status::playerFlag_[232] = {0};

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


    return *(reinterpret_cast<uint16_t*>(record) + 0x1B);
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

void status::PlayerData::setNameString(PlayerData* self, char* name)
{
    std::strncpy(self->name_, name, sizeof(self->name_) - 1);
    self->name_[sizeof(self->name_) - 1] = '\0';
}


int status::PlayerData::getCgIndex(PlayerData* self) {
   
    uint16_t* record = reinterpret_cast<uint16_t*>(dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_))
    );

    switch (self->kindIndex_) {
    case 7:
        if (status::HaveEquipment::isEquipment(&self->haveEquipment_, 69)) {
            return 221;
        }
        break;

    case 1:
        return (status::ChapterSet_ == 1) ? 200 : 201;

    case 14:
        return status::HaveStatusInfo::getDaughterCharaIndex();

    case 13:
        return status::HaveStatusInfo::getSonCharaIndex();
    }

    return record[24];
}


int status::PlayerData::getIconIndex(PlayerData* self) {

    uint8_t* record = reinterpret_cast<uint8_t*>(dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_))
        );


    // Cas spécifiques : personnages enfants
    if (self->kindIndex_ == 14)
        return status::HaveStatusInfo::getDaughterIconIndex();

    if (self->kindIndex_ == 13)
        return status::HaveStatusInfo::getSonIconIndex();

    return record[79];
}


int status::PlayerData::getEquipAttrIndex(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );


    return *(reinterpret_cast<uint8_t*>(record) + 0x4E);
}

uint8_t status::PlayerData::getLevelMax(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );


    return *(reinterpret_cast<uint8_t*>(record) + 0x50);
}

uint16_t status::PlayerData::getJob(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );


    return *(reinterpret_cast<uint16_t*>(record) + 0x19);
}

uint16_t status::PlayerData::getMonsterIndex(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );


    return *(reinterpret_cast<uint16_t*>(record) + 0x1C);
}

uint16_t status::PlayerData::getSexId(PlayerData* self)
{
    void* record = dq5::level::CharacterInitData::binary_.getRecord(
        self->kindIndex_,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
    );


    return *(reinterpret_cast<uint16_t*>(record) + 0x1A);
}

void status::PlayerData::setupNameId(PlayerData* self, int kindIndex, int offset) {
    // Test : kindIndex ∈ [58, 82] et bit mask autorisé
    if (static_cast<unsigned int>(kindIndex - 58) > 0x18 || ((1 << (kindIndex - 58)) & 0x1000003) == 0)
        return;

    // Lecture de nameId à partir de l'enregistrement binaire (champ WORD à l'offset 2)
    uint16_t* record = reinterpret_cast<uint16_t*>(dq5::level::CharacterInitData::binary_.getRecord(
        kindIndex,
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_))
    );

    uint16_t nameId = record[2] + offset;
    self->nameId_ = nameId;

   
    /* need to do ardq::TextAPI::extractText(self->name_, 0x05000000, nameId + 600);


    char macroState = ardq::getMacroState();
    if (macroState & 0x01)
        self->nameBasedSex_ = SEX_MALE;
    if (macroState & 0x02)
        self->nameBasedSex_ = SEX_FEMALE;*/
}


void status::PlayerData::setup(PlayerData* self, bool useNameId, int kind, int playerIndex, int offset) {
    if (kind == 0)
        return;

    self->index_ = playerIndex;
    self->flag_.reset();
    self->kindIndex_ = kind;

    uint8_t* record = reinterpret_cast<uint8_t*>(dq5::level::CharacterInitData::binary_.getRecord(
        static_cast<uint8_t>(kind),
        dq5::level::CharacterInitData::addr_,
        dq5::level::CharacterInitData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_))
        );

    status::BaseStatus::clear(&self->baseStatus_);

    // Nom
    uint16_t rawNameId = *reinterpret_cast<uint16_t*>(record + 4); // Record[2]
    if (rawNameId) {
        self->nameId_ = static_cast<uint8_t>(rawNameId + offset);
        if (useNameId)
            self->setupNameId(self,self->kindIndex_, offset);
    }
    else {
        uint8_t finalNameId = static_cast<uint8_t>(playerIndex);
        self->nameId_ = finalNameId;

        if (useNameId) {
            if (playerIndex != kind &&
                (kind >= 58 && kind <= 58 + 24) &&
                ((1 << (kind - 58)) & 0x1000003)) {
                finalNameId = kind;
                self->nameId_ = finalNameId;
            }

            /*ardq::TextAPI::extractText(self->name_, 0x5000000, finalNameId + 500);
            char macro = ardq::getMacroState();
            if (macro & 1) self->nameBasedSex_ = dq5::Sex::SEX_MALE;
            if (macro & 2) self->nameBasedSex_ = dq5::Sex::SEX_FEMALE;*/
        }
    }

    // Niveau
    uint8_t level = record[81];
    self->baseStatus_.level_ = level;

    status::BaseStatus::setStrength(&self->baseStatus_, record[8]);
    status::BaseStatus::setAgility(&self->baseStatus_, record[10]);
    status::BaseStatus::setProtection(&self->baseStatus_, record[12]);
    status::BaseStatus::setWisdom(&self->baseStatus_, record[14]);
    status::BaseStatus::setLuck(&self->baseStatus_, record[16]);

    uint16_t hp = *reinterpret_cast<uint16_t*>(record + 18);
    status::BaseStatus::setHpMax(&self->baseStatus_, hp);
    status::BaseStatus::setHp(&self->baseStatus_, hp);

    uint16_t mp = *reinterpret_cast<uint16_t*>(record + 20);
    status::BaseStatus::setMpMax(&self->baseStatus_, mp);
    status::BaseStatus::setMp(&self->baseStatus_, mp);

    self->baseStatus_.exp_ = *reinterpret_cast<uint32_t*>(record + 0);

    self->haveItem_.VBaseHaveItemClear();


    // Objets initiaux (équipements)
    for (int i = 0; i < 4; ++i) {
        uint16_t itemId = *reinterpret_cast<uint16_t*>(record + 24 + 2 * i);
        if (itemId != 0) {
            int slot = self->haveItem_.VHaveItemAdd(itemId);
            status::BaseHaveItem::setEquipment(&self->haveItem_, slot);
        }
    }

    status::HaveAction::clear(&self->haveAction_);
    status::HaveAction::setup(&self->haveAction_, self->index_, 1);
    self->haveAction_.level_ = level;

    // Compétences / sorts initiaux
    struct { int16_t id; int16_t param; } skills[] = {
        { *reinterpret_cast<int16_t*>(record + 32), *reinterpret_cast<int16_t*>(record + 40) },
        { *reinterpret_cast<int16_t*>(record + 34), *reinterpret_cast<int16_t*>(record + 42) },
        { *reinterpret_cast<int16_t*>(record + 36), *reinterpret_cast<int16_t*>(record + 44) },
        { *reinterpret_cast<int16_t*>(record + 58), *reinterpret_cast<int16_t*>(record + 68) },
        { *reinterpret_cast<int16_t*>(record + 60), *reinterpret_cast<int16_t*>(record + 70) },
        { *reinterpret_cast<int16_t*>(record + 62), *reinterpret_cast<int16_t*>(record + 72) },
        { *reinterpret_cast<int16_t*>(record + 64), *reinterpret_cast<int16_t*>(record + 74) },
        { *reinterpret_cast<int16_t*>(record + 66), *reinterpret_cast<int16_t*>(record + 76) },
    };

    for (auto& skill : skills) {
        if (skill.id != 0 || skill.param != 0)
            status::HaveAction::add(&self->haveAction_, skill.id, skill.param, 1);
    }

    // Équipement
    status::HaveEquipment::setup(&self->haveEquipment_, &self->haveItem_);

    self->command_ = 5;
}
