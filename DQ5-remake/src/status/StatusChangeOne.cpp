#include "status/StatusChangeOne.h"
#include "ar/BitFlag.h"
#include "ar/rand.h"
#include "dq5/ActionParam.h"
#include "dq5/ActionTypeList.h"
#include "dq5/EffectMessage.h"

int status::actionType_;
uintptr_t status::effectMessage_;

bool status::StatusChangeOne::isEnable(const StatusChangeOne* self) {
    return self->flag_.test(0);
}


void status::StatusChangeOne::StatusChangeOneInit(StatusChangeOne* self) {
    self->astron_ = 0;
    *reinterpret_cast<uint32_t*>(&self->damageType_) = 0;  // brut si tu sais que les 4 octets suivants sont à zéro
    self->flag_.setValue(0);
}

void status::StatusChangeOne::clear(StatusChangeOne* self) {
    self->flag_.setValue(0);
    self->count_ = 0;
    self->turn_ = 0;
    self->pattern_ = 0;

}

void status::StatusChangeOne::setup(StatusChangeOne* self, dq5::level::ActionParam::ACTIONTYPE release,int actionIndex,bool flag)
{
    self->pattern_ = 0;
    self->flag_.setValue(1);
    self->damageType_ = static_cast<uint8_t>(release);
    self->actionIndex_ = actionIndex;
    self->count_ = 0;
    self->turn_ = 0;

    // Lecture du record dans la table ActionTypeList
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        static_cast<uint8_t>(release),
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return;

    auto* entry = static_cast<dq5::level::ActionTypeList*>(record);
    actionType_ = reinterpret_cast<int>(entry); 

    self->turn_ = entry->turn;
    self->pattern_ = entry->pattern;

    if (flag) {
        self->turn_ += status::StatusChangeOne::addTurn(self);
    }
}

int status::StatusChangeOne::addTurn(StatusChangeOne* self) {
    switch (self->pattern_) {
    case 1:
        return ar::rand(1) + 1;

    case 2: {
        if (ar::rand(8) < 1)
            return 1;
        if (ar::rand(8) < 3)
            return 2;
        if (ar::rand(8) < 5)
            return 3;
        ar::rand(8);
        return 4;
    }

    case 3: {
        if (ar::rand(8) < 3)
            return 1;
        if (ar::rand(8) < 5)
            return 2;
        if (ar::rand(8) < 7)
            return 3;
        ar::rand(8);
        return 4;
    }

    case 4: {
        if (ar::rand(8) < 5)
            return 1;
        if (ar::rand(8) < 6)
            return 2;
        if (ar::rand(8) < 7)
            return 3;
        ar::rand(8);
        return 4;
    }

    case 5:
        return 1;

    case 6:
        return 128;

    default:
        return 0;
    }
}


bool status::StatusChangeOne::isValidOnAstron(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    uint8_t flags = *reinterpret_cast<uint8_t*>(status::actionType_ + 3);

    return ((flags & 0x40) >> 6) ^ 1;
}


bool status::StatusChangeOne::isValidOnSpazz(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    int8_t byte3 = *reinterpret_cast<int8_t*>(status::actionType_ + 3);
    return byte3 >= 0;
}

bool status::StatusChangeOne::isAfterOnSleep(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    uint8_t byte4 = *reinterpret_cast<uint8_t*>(status::actionType_ + 4);

    // Bit 1 (masque 0x02), puis inverse (XOR 1)
    return ((byte4 & 0x02) >> 1) ^ 1;
}


bool status::StatusChangeOne::isAfterOnPath1(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    uint8_t byte4 = *reinterpret_cast<uint8_t*>(status::actionType_ + 4);

    // Test du bit 2 (0x04), puis inversion XOR 1
    return ((byte4 & 0x04) >> 2) ^ 1;
}


bool status::StatusChangeOne::isAfterOnConfuze(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    uint8_t byte4 = *reinterpret_cast<uint8_t*>(status::actionType_ + 4);

    // Vérifie si le bit 3 (valeur 0x08) est désactivé (0 → true)
    return ((byte4 & 0x08) >> 3) ^ 1;
}


bool status::StatusChangeOne::isAfterOnPowerSave(int damageType) {
    void* record = dq5::level::ActionTypeList::binary_.getRecord(
        damageType,
        dq5::level::ActionTypeList::addr_,
        dq5::level::ActionTypeList::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
    );

    if (!record) return false;

    status::actionType_ = reinterpret_cast<std::uintptr_t>(record);
    uint8_t byte4 = *reinterpret_cast<uint8_t*>(status::actionType_ + 4);

    // Vérifie si le bit 4 (0x10) est désactivé (false → peut être utilisé après PowerSave)
    return ((byte4 & 0x10) >> 4) ^ 1;
}


void status::StatusChangeOne::cleanup(StatusChangeOne* self) {
    self->flag_.setValue(0);        // Réinitialise le flag (équivalent à flag_.flag_ = 0)
    self->count_ = 0;
    self->pattern_ = 0;
}


bool status::StatusChangeOne::isCancel(StatusChangeOne* self){
    if (self->flag_.get() & 0x01) {
        return self->count_ >= self->turn_;
    }
    return false;
}


void status::StatusChangeOne::execStartOfTurn(StatusChangeOne* self) {
    if (self->flag_.get() & 0x01) {
        void* record = dq5::level::ActionTypeList::binary_.getRecord(
            self->damageType_,
            dq5::level::ActionTypeList::addr_,
            dq5::level::ActionTypeList::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
        );

        status::actionType_ = reinterpret_cast<intptr_t>(record);
        if (*(reinterpret_cast<uint8_t*>(record) + 3) & 0x02)
            ++self->count_;
    }
}

void status::StatusChangeOne::setEnable(StatusChangeOne* self, bool flag) {
    uint32_t current = self->flag_.get() & ~0x01; // Clear bit 0
    if (flag) {
        current |= 0x01; 
    }
    self->flag_.setValue(current);
}

void status::StatusChangeOne::setRelease(StatusChangeOne* self, bool flag) {
    uint32_t current = self->flag_.get() & ~0x02; // Efface le bit 1
    if (flag) {
        current |= 0x02; // Active le bit 1 si flag est vrai
    }
    self->flag_.setValue(current);
}

void status::StatusChangeOne::execEndOfTurn(StatusChangeOne* self) {
    uint8_t flag = self->flag_.get();

    // Vérifie si l'effet est actif (bit 0) et que le nombre de tours n'est pas infini (255)
    if ((flag & 0x01) && self->turn_ != 255) {
        void* record = dq5::level::ActionTypeList::binary_.getRecord(
            self->damageType_,
            dq5::level::ActionTypeList::addr_,
            dq5::level::ActionTypeList::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
        );

        if (!record) return;

        status::actionType_ = reinterpret_cast<int>(record);

        // Si le 3ème octet (offset +3) a le bit 2 activé (0x04), on incrémente le compteur
        if ((reinterpret_cast<uint8_t*>(record)[3] & 0x04) != 0) {
            ++self->count_;
        }

        if (self->astron_) {
            ++self->count_;
        }
    }
}


void status::StatusChangeOne::execEndOfRound(StatusChangeOne* self) {
    uint8_t flag = self->flag_.get();

    // Vérifie si le statut est actif et que la durée n'est pas infinie
    if ((flag & 0x01) && self->turn_ != 255) {
        void* record = dq5::level::ActionTypeList::binary_.getRecord(
            self->damageType_,
            dq5::level::ActionTypeList::addr_,
            dq5::level::ActionTypeList::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ActionTypeList::loadSwitch_)
        );

        if (!record) return;

        status::actionType_ = reinterpret_cast<int>(record);

        // Si le 4ème octet (offset +3) a le bit 3 activé (0x08), on incrémente le compteur
        if ((reinterpret_cast<uint8_t*>(record)[3] & 0x08) != 0) {
            ++self->count_;
        }
    }
}

void status::StatusChangeOne::execWalk(StatusChangeOne* self) {
    if ((self->flag_.get() & 0x01) && self->turn_ != 255) {
        ++self->count_;
    }
}


bool status::StatusChangeOne::isRelease(const StatusChangeOne* self) {
    return (self->flag_.get() >> 1) & 0x01;
}


int status::StatusChangeOne::getExecMessage(StatusChangeOne* self) {
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return status::effectMessage_;
}


int status::StatusChangeOne::getExecMessage2(StatusChangeOne* self) {

    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);

    return (status::effectMessage_ + 4);
}


int status::StatusChangeOne::getResultPlayerMessage(StatusChangeOne* self)
{
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return (status::effectMessage_ + 8);
}


int status::StatusChangeOne::getResultMonsterMessage(StatusChangeOne* self)
{
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return (status::effectMessage_ + 12);
}


int status::StatusChangeOne::getAgainEnableMessage(StatusChangeOne* self)
{
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return (status::effectMessage_ + 16);
}

int status::StatusChangeOne::getAgainDisableMessage(StatusChangeOne* self)
{
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return (status::effectMessage_ + 20);
}

int status::StatusChangeOne::getReleaseMessage(StatusChangeOne* self)
{
    status::StatusChangeOne::getEffectMessage(self->damageType_, self->actionIndex_);
    return (status::effectMessage_ + 24);
}


int status::StatusChangeOne::getEffectMessage(int index, int actionIndex) {
    using namespace dq5::level;

    int count = 0;
    int matchedIndex = -1;
    EffectMessage* matchedRecord = nullptr;

    auto getRecord = [](unsigned int i) -> EffectMessage* {
        return static_cast<EffectMessage*>(
            EffectMessage::binary_.getRecord(
                i,
                EffectMessage::addr_,
                EffectMessage::filename_[0],
                static_cast<ar::File::LoadSwitch>(EffectMessage::loadSwitch_)
            )
            );
        };

    // Première passe : match exact (typeID + action)
    for (unsigned int i = 0; i < count; ++i) {
        EffectMessage* em = getRecord(i);
        if (em->typeID == index && em->action == actionIndex) {
            matchedIndex = i;
            matchedRecord = em;
            break;
        }
    }

    // Deuxième passe : typeID + action = 0
    if (!matchedRecord) {
        for (unsigned int i = 0; i < count; ++i) {
            EffectMessage* em = getRecord(i);
            if (em->typeID == index && em->action == 0) {
                matchedIndex = i;
                matchedRecord = em;
                break;
            }
        }
    }

    // Troisième passe : typeID seul
    if (!matchedRecord) {
        for (unsigned int i = 0; i < count; ++i) {
            EffectMessage* em = getRecord(i);
            if (em->typeID == index) {
                matchedIndex = i;
                matchedRecord = em;
                break;
            }
        }
    }

    // Fallback : premier record
    if (!matchedRecord) {
        matchedIndex = 0;
        matchedRecord = getRecord(0);
    }

    status::effectMessage_ = reinterpret_cast<std::uintptr_t>(matchedRecord);

    return matchedIndex;
}

status::StatusChangeOne::~StatusChangeOne()
{
}
