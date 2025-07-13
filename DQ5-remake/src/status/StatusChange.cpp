#include "status/HaveStatusInfo.h"
#include "status/HaveStatusInfo_0.h"
#include "status/StatusChange.h"
#include "status/PlayerData.h"
#include "status/PlayerDataAll.h"
#include "status/StatusChangeOne.h"
#include "status/StatusChange.h"
#include "dq5/ActionParam.h"
#include "ar/BitFlag.h"



status::StatusChangeOne status::StatusChange::statusFizzleZone_;
status::StatusChangeOne status::StatusChange::statusCloseDoor_;
status::StatusChangeOne status::StatusChange::statusToherosu_;
status::StatusChangeOne status::StatusChange::statusSinobiasi_;
status::StatusChangeOne status::StatusChange::statusMonstersFood_;
status::StatusChangeOne status::StatusChange::statusTimeStop_;

uint8_t status::TIMESTOP = 0;

uint8_t status::StatusChange::ToherosCount_ = 0;
uint16_t status::StatusChange::ToherosTurn_ = 0;

uint8_t status::StatusChange::SinobiasCount_ = 0;
uint16_t status::StatusChange::SinobiasTurn_ = 0;

uint8_t status::StatusChange::MonstersFoodCount_ = 0;
uint16_t status::StatusChange::MonstersFoodTurn_ = 0;



bool status::StatusChange::isEnable(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    

    const status::StatusChangeOne* ptr = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        ptr = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        ptr = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        ptr = &status::StatusChange::statusSinobiasi_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        ptr = &status::StatusChange::statusMonstersFood_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        ptr = &status::StatusChange::statusCloseDoor_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        ptr = &status::StatusChange::statusTimeStop_;
        break;

    default:
        ptr = &self->status_[static_cast<int>(rel)];
        break;
    }

    return status::StatusChangeOne::isEnable(ptr);
}


void status::StatusChange::setup(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE re, bool flag) {
  
    if (!status::StatusChange::isValidBefore(self, static_cast<int>(re)))
        return;

    status::StatusChangeOne* target = nullptr;

    switch (re) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        target = &StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        target = &StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        target = &StatusChange::statusSinobiasi_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        target = &StatusChange::statusMonstersFood_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        target = &StatusChange::statusCloseDoor_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        target = &StatusChange::statusTimeStop_;
        break;

    default:
        target = &self->status_[static_cast<int>(re)];
        break;
    }

    // Appelle setup sur la cible déterminée
    status::StatusChangeOne::setup(target, re, 0, flag);
    status::StatusChange::checkValidAfter(self, static_cast<int>(re));
}


bool status::StatusChange::isValidBefore(StatusChange* self, int damageType) {
    const auto& statusAstron = self->status_[18];
    const auto& statusSpazz = self->status_[19];

    // Si l'effet "Astron" n'est pas actif
    if (!status::StatusChangeOne::isEnable(&statusAstron)) {
        // On permet si : Spazz désactivé OU damageType valide pour Spazz
        return !status::StatusChangeOne::isEnable(&statusSpazz) || status::StatusChangeOne::isValidOnSpazz(damageType);
    }

    // Sinon, Astron est actif : il faut que le type soit valide pour Astron
    if (status::StatusChangeOne::isValidOnAstron(damageType)) {
        return !StatusChangeOne::isEnable(&statusSpazz) || status::StatusChangeOne::isValidOnSpazz(damageType);
    }

    // Sinon, rejeté
    return false;
}


void status::StatusChange::checkValidAfter(StatusChange* self, int damageType) {
   
    // SLEEP (index 20)
    if (status::StatusChangeOne::isEnable(&self->status_[20]) &&
        !status::StatusChangeOne::isAfterOnSleep(damageType)) {
        status::StatusChangeOne::clear(&self->status_[20]);
    }

    // PATH1 (index 32)
    if (status::StatusChangeOne::isEnable(&self->status_[32]) &&
        !status::StatusChangeOne::isAfterOnPath1(damageType)) {
        status::StatusChangeOne::clear(&self->status_[32]);
    }

    // CONFUZE (index 31)
    if (status::StatusChangeOne::isEnable(&self->status_[31]) &&
        !status::StatusChangeOne::isAfterOnConfuze(damageType)) {
        status::StatusChangeOne::clear(&self->status_[31]);
    }

    // POWERSAVE (index 28)
    if (status::StatusChangeOne::isEnable(&self->status_[28]) &&
        !status::StatusChangeOne::isAfterOnPowerSave(damageType)) {
        status::StatusChangeOne::clear(&self->status_[28]);
    }
}


void status::StatusChange::release(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {

    status::StatusChangeOne* target = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        target = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        target = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        status::StatusChangeOne::clear(&status::StatusChange::statusSinobiasi_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        status::StatusChangeOne::clear(&status::StatusChange::statusMonstersFood_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        status::StatusChangeOne::clear(&status::StatusChange::statusCloseDoor_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        target = &status::StatusChange::statusTimeStop_;
        break;

    default:
        target = &self->status_[static_cast<int>(rel)];
        break;
    }

    if (target) {
        status::StatusChangeOne::clear(target);
    }
}


bool status::StatusChange::isRelease(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    const status::StatusChangeOne* target = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        target = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        target = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        return status::StatusChangeOne::isRelease(&status::StatusChange::statusSinobiasi_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        return status::StatusChangeOne::isRelease(&status::StatusChange::statusMonstersFood_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        return status::StatusChangeOne::isRelease(&status::StatusChange::statusCloseDoor_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        target = &status::StatusChange::statusTimeStop_;
        break;

    default:
        target = &self->status_[static_cast<int>(rel)];
        break;
    }

    return status::StatusChangeOne::isRelease(target);
}


int status::StatusChange::getActionIndex(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* v2 = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        v2 = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        v2 = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        v2 = &status::StatusChange::statusSinobiasi_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        v2 = &status::StatusChange::statusMonstersFood_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        v2 = &status::StatusChange::statusCloseDoor_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        v2 = &status::StatusChange::statusTimeStop_;
        break;

    default:
        v2 = &self->status_[static_cast<int>(rel)];
        break;
    }

    return v2->actionIndex_;
}

int status::StatusChange::getReleaseMessage(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* v3 = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        v3 = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        v3 = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        return status::StatusChangeOne::getReleaseMessage(&status::StatusChange::statusSinobiasi_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        return status::StatusChangeOne::getReleaseMessage(&status::StatusChange::statusMonstersFood_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        return status::StatusChangeOne::getReleaseMessage(&status::StatusChange::statusCloseDoor_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        v3 = &status::StatusChange::statusTimeStop_;
        break;

    default:
        v3 = &self->status_[static_cast<int>(rel)];
        break;
    }

    return status::StatusChangeOne::getReleaseMessage(v3);
}


void status::StatusChange::cleanup(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* target = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        target = &status::StatusChange::statusFizzleZone_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        target = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        status::StatusChangeOne::cleanup(&status::StatusChange::statusSinobiasi_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        status::StatusChangeOne::cleanup(&status::StatusChange::statusMonstersFood_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        status::StatusChangeOne::cleanup(&status::StatusChange::statusCloseDoor_);
        return;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        target = &status::StatusChange::statusTimeStop_;
        break;

    default:
        target = &self->status_[static_cast<int>(rel)];
        break;
    }

    if (target != nullptr) {
        status::StatusChangeOne::cleanup(target);
    }
}


void status::StatusChange::StatusChangeInit(StatusChange* self)
{
    for (int i = 0; i != 51; ++i)
        status::StatusChangeOne::StatusChangeOne(self->status_[i]);
}

status::StatusChange::~StatusChange() {
    for (int i = 50; i >= 0; --i) {
        this->status_[i].~StatusChangeOne();
    }
}

void status::StatusChange::restore(StatusChange* self, int index) {
    // Nettoie tous les statuts avant de restaurer
    status::StatusChange::clear(self);

    // Accès à PlayerData via PlayerDataAll::playerData_ + 276 * index
    status::PlayerData* player = reinterpret_cast<status::PlayerData*>(
        reinterpret_cast<char*>(&status::PlayerDataAll::playerData_) + 276 * index
        );

    // Restauration conditionnelle des statuts depuis les flags du joueur
    if (status::PlayerData::isCurse4(player)) {
        status::StatusChange::setup(self, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4, false);
    }

    if (status::PlayerData::isCurse5(player)) {
        status::StatusChange::setup(self, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5, false);
    }

    if (status::PlayerData::isPoison(player)) {
        status::StatusChange::setup(self, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU, false);
    }

    if (status::PlayerData::isVenom(player)) {
        status::StatusChange::setup(self, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MOUDOKU, false);
    }
}

void status::StatusChange::clear(StatusChange* self) {
    for (int i = 0; i < 51; ++i) {
        // Clear all status effects except a selected set between indices 35–47 based on bitmask 0x1341
        if (i < 35 || i > 47 || ((1 << (i - 35)) & 0x1341) == 0) {
            if (i == 42) {
                status::StatusChangeOne::clear(&status::StatusChange::statusTimeStop_);
            }
            else {
                status::StatusChangeOne::clear(&self->status_[i]);
            }
        }
    }
}

void status::StatusChange::store(StatusChange* self, int index) {
    // Accès brut à PlayerData via l'adresse de base + 276 * index
    status::PlayerData* player = reinterpret_cast<status::PlayerData*>(
        reinterpret_cast<char*>(&status::PlayerDataAll::playerData_) + 276 * index
        );

    // Lecture des statuts dans le tableau status_ de StatusChange
    bool isCurse4 = status::StatusChangeOne::isEnable(&self->status_[45]);
    status::PlayerData::setCurse4(player, isCurse4);

    bool isCurse5 = status::StatusChangeOne::isEnable(&self->status_[46]);
    status::PlayerData::setCurse5(player, isCurse5);

    bool isPoison = status::StatusChangeOne::isEnable(&self->status_[33]);
    status::PlayerData::setPoison(player, isPoison);

    bool isVenom = status::StatusChangeOne::isEnable(&self->status_[34]);
    status::PlayerData::setVenom(player, isVenom);
}


bool status::StatusChange::setup(StatusChange* self, int actionIndex, bool flag) {
    
    // Récupération de l'enregistrement Excel correspondant
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        reinterpret_cast<void*>(dq5::level::ActionParam::addr_),
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    // Lecture du type d'action (byte à l'offset +54)
    uint8_t actionTypeValue = *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(record) + 54);
    dq5::level::ActionParam::ACTIONTYPE actionType = static_cast<dq5::level::ActionParam::ACTIONTYPE>(actionTypeValue);

    // Vérifie la validité avant application
    if (!status::StatusChange::isValidBefore(self, static_cast<int>(actionType)))
        return false;

    // Détermine le pointeur vers le slot de statut à modifier
    status::StatusChangeOne* statusPtr = nullptr;
    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusPtr = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        statusPtr = &status::StatusChange::statusTimeStop_;
        break;
    default:
        statusPtr = &self->status_[static_cast<int>(actionType)];
        break;
    }

    status::StatusChangeOne::setup(statusPtr, actionType, actionIndex, flag);

    if (actionType > dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_KONRAN) {
        if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP) {
            status::TIMESTOP = 1;
        }
        else if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_PATH_1) {
            statusPtr->turn_ = (actionIndex == 405) ? 2 : 1;
        }
    }
    else if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_ASTORON) {
        statusPtr->astron_ = 1;
    }
    else if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TAMEKEI) {
        statusPtr->turn_ = 2;
    }

    status::StatusChange::checkValidAfter(self, static_cast<int>(actionType));
    return true;
}


void status::StatusChange::execWalk(status::StatusChange* self) {
    for (int i = 0; i < 51; ++i) {
        int actionTypeValue = i & 0x7FFFFFFF;  // Fidèle au binaire

        status::StatusChange* statusPtr = nullptr;

        switch (actionTypeValue) {
        case ')':
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
            break;
        case '+':
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
            break;
        case ',':
            status::StatusChangeOne::execWalk(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
            break;
        case '/':
            status::StatusChangeOne::execWalk(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
            break;
        default:
            if (actionTypeValue == 35) {
                statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
            }
            else {
                statusPtr = self;
                if (i == 42)
                    statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
            }
            break;
        }

        // Exécute le statut
        status::StatusChangeOne::execWalk(statusPtr->status_);

        // Si le statut a été annulé, on le réinitialise et marque comme libéré
        if (status::StatusChangeOne::isCancel(statusPtr->status_)) {
            status::StatusChange* clearPtr = nullptr;

            switch (actionTypeValue) {
            case ')':
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
                break;
            case '+':
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
                break;
            case ',':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_), false);
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
                break;
            case '/':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_), false);
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
                break;
            default:
                if (actionTypeValue == 35) {
                    clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
                }
                else {
                    clearPtr = self;
                    if (i == 42)
                        clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
                }
                break;
            }

            status::StatusChangeOne::setEnable(clearPtr->status_, false);
            status::StatusChangeOne::setRelease(clearPtr->status_, true);
        }

        self = reinterpret_cast<status::StatusChange*>(reinterpret_cast<char*>(self) + 0xC);
    }
}



void status::StatusChange::execStartOfTurn(StatusChange* self) {
    for (int i = 0; i < 51; ++i) {
        status::StatusChange* target = self;

        int actionTypeValue = i & 0x7FFFFFFF;

        switch (actionTypeValue) {
        case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE):
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
            break;
        case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU):
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
            break;
        case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI):
            status::StatusChangeOne::execStartOfTurn(&status::StatusChange::statusSinobiasi_);
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
            break;
        case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA):
            status::StatusChangeOne::execStartOfTurn(&status::StatusChange::statusMonstersFood_);
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
            break;
        case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR):
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
            break;
        default:
            if (actionTypeValue == static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)) {
                target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
            }
            break;
        }

        status::StatusChangeOne::execStartOfTurn(target->status_);

        if (status::StatusChangeOne::isCancel(target->status_)) {
            status::StatusChange* toReset = self;

            switch (actionTypeValue) {
            case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE):
                toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
                break;
            case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU):
                toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
                break;
            case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI):
                status::StatusChangeOne::setEnable(&status::StatusChange::statusSinobiasi_, false);
                toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
                break;
            case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA):
                status::StatusChangeOne::setEnable(&status::StatusChange::statusMonstersFood_, false);
                toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
                break;
            case static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR):
                toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
                break;
            default:
                if (actionTypeValue == static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)) {
                    toReset = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
                }
                break;
            }

            status::StatusChangeOne::setEnable(toReset->status_, false);
            status::StatusChangeOne::setRelease(toReset->status_, true);
        }

        self = reinterpret_cast<status::StatusChange*>(
            reinterpret_cast<char*>(self) + sizeof(status::StatusChangeOne)
            );
    }
}

void status::StatusChange::execEndOfTurn(StatusChange* self) {
    for (int i = 0; i < 51; ++i) {
        int actionTypeValue = i & 0x7FFFFFFF;

        status::StatusChange* statusPtr = nullptr;

        switch (actionTypeValue) {
        case ')':
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
            break;
        case '+':
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
            break;
        case ',':
            status::StatusChangeOne::execEndOfTurn(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
            break;
        case '/':
            status::StatusChangeOne::execEndOfTurn(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
            break;
        default:
            if (actionTypeValue == 35) {
                statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
            }
            else {
                statusPtr = self;
                if (i == 42)
                    statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
            }
            break;
        }

        status::StatusChangeOne::execEndOfTurn(statusPtr->status_);

        if (status::StatusChangeOne::isCancel(statusPtr->status_)) {
            status::StatusChange* clearPtr = nullptr;

            switch (actionTypeValue) {
            case ')':
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
                break;
            case '+':
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusToherosu_);
                break;
            case ',':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_), false);
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
                break;
            case '/':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_), false);
                clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
                break;
            default:
                if (actionTypeValue == 35) {
                    clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
                }
                else {
                    clearPtr = self;
                    if (i == 42)
                        clearPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
                }
                break;
            }

            status::StatusChangeOne::setEnable(clearPtr->status_, false);
            status::StatusChangeOne::setRelease(clearPtr->status_, true);
        }

        self = reinterpret_cast<status::StatusChange*>(reinterpret_cast<char*>(self) + 0xC);
    }
}

void status::StatusChange::execEndOfRound(StatusChange* self) {
    for (int i = 0; i < 51; ++i) {
        if (i == 42) {
            self = reinterpret_cast<status::StatusChange*>(reinterpret_cast<char*>(self) + 0xC);
            continue;
        }

        int actionTypeValue = i & 0x7FFFFFFF;
        status::StatusChangeOne* statusPtrOne = nullptr;
        status::StatusChange* statusPtr = nullptr;

        switch (actionTypeValue) {
        case ')':
            statusPtrOne = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusFizzleZone_);
            status::StatusChangeOne::execEndOfRound(statusPtrOne);
            statusPtr = reinterpret_cast<status::StatusChange*>(statusPtrOne);
            break;
        case '+':
            statusPtrOne = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusToherosu_);
            status::StatusChangeOne::execEndOfRound(statusPtrOne);
            statusPtr = reinterpret_cast<status::StatusChange*>(statusPtrOne);
            break;
        case ',':
            status::StatusChangeOne::execEndOfRound(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
            break;
        case '/':
            status::StatusChangeOne::execEndOfRound(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_));
            statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
            break;
        default:
            if (actionTypeValue == 35) {
                status::StatusChangeOne::execEndOfRound(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusCloseDoor_));
                statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
            }
            else {
                status::StatusChangeOne::execEndOfRound(self->status_);
                statusPtr = self;
            }
            break;
        }

        if (status::StatusChangeOne::isCancel(statusPtr->status_)) {
            switch (actionTypeValue) {
            case ')':
                statusPtrOne = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusFizzleZone_);
                status::StatusChangeOne::setEnable(statusPtrOne, false);
                statusPtr = reinterpret_cast<status::StatusChange*>(statusPtrOne);
                break;
            case '+':
                statusPtrOne = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusToherosu_);
                status::StatusChangeOne::setEnable(statusPtrOne, false);
                statusPtr = reinterpret_cast<status::StatusChange*>(statusPtrOne);
                break;
            case ',':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_), false);
                statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusSinobiasi_);
                break;
            case '/':
                status::StatusChangeOne::setEnable(reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_), false);
                statusPtr = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusMonstersFood_);
                break;
            default:
                if (actionTypeValue == 35) {
                    statusPtrOne = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusCloseDoor_);
                    status::StatusChangeOne::setEnable(statusPtrOne, false);
                    statusPtr = reinterpret_cast<status::StatusChange*>(statusPtrOne);
                }
                else {
                    status::StatusChangeOne::setEnable(self->status_, false);
                    statusPtr = self;
                }
                break;
            }

            status::StatusChangeOne::setRelease(statusPtr->status_, true);
        }

        self = reinterpret_cast<status::StatusChange*>(reinterpret_cast<char*>(self) + 0xC);
    }
}


int status::StatusChange::getExecMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusFizzleZone_);

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        return status::StatusChangeOne::getExecMessage(statusPtr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusToherosu_);
        return status::StatusChangeOne::getExecMessage(statusPtr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        return status::StatusChangeOne::getExecMessage(
            reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_));

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        return status::StatusChangeOne::getExecMessage(
            reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_));

    default:
        if (rel == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR) {
            return status::StatusChangeOne::getExecMessage(
                reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusCloseDoor_));
        }

        // Pour les cas TIMESTOP, CURSE4, CURSE5 ou autre
        statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusTimeStop_);
        if (rel != dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP) {
            statusPtr = &self->status_[static_cast<int>(rel)];
        }
        return status::StatusChangeOne::getExecMessage(statusPtr);
    }
}


int status::StatusChange::getExecMessage2(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusFizzleZone_);

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        return status::StatusChangeOne::getExecMessage2(statusPtr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusToherosu_);
        return status::StatusChangeOne::getExecMessage2(statusPtr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        return status::StatusChangeOne::getExecMessage2(
            reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusSinobiasi_));

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        return status::StatusChangeOne::getExecMessage2(
            reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusMonstersFood_));

    default:
        if (rel == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR) {
            return status::StatusChangeOne::getExecMessage2(
                reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusCloseDoor_));
        }

        // Pour TIMESTOP, CURSE4, CURSE5 ou autre
        statusPtr = reinterpret_cast<status::StatusChangeOne*>(&status::StatusChange::statusTimeStop_);
        if (rel != dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP) {
            statusPtr = &self->status_[static_cast<int>(rel)];
        }
        return status::StatusChangeOne::getExecMessage2(statusPtr);
    }
}


int status::StatusChange::getResultMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType, dq5::level::CharacterType characterType) {
    const status::StatusChangeOne* statusSlot = nullptr;

    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusSlot = &StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        // handled after default
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusSlot = &StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusSlot = &StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusSlot = &StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusSlot = &StatusChange::statusCloseDoor_;
        break;
    default:
        statusSlot = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
        break;
    }

    if (!status::StatusChangeOne::isEnable(statusSlot))
        return 0;

    // Calcul de l’index relatif par rapport à ACTIONTYPE_FIZZLEZONE (valeur 41)
    const int relativeIndex = static_cast<int>(actionType) - 41;

    if (characterType == dq5::level::CharacterType::PLAYER) {
        switch (relativeIndex) {
        case 0:
            return StatusChangeOne::getResultPlayerMessage(&StatusChange::statusFizzleZone_);
        case 2:
            return StatusChangeOne::getResultPlayerMessage(&StatusChange::statusToherosu_);
        case 3:
            return StatusChangeOne::getResultPlayerMessage(&StatusChange::statusSinobiasi_);
        case 6:
            return StatusChangeOne::getResultPlayerMessage(&StatusChange::statusMonstersFood_);
        default:
            if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR)
                return StatusChangeOne::getResultPlayerMessage(&StatusChange::statusCloseDoor_);
            else
                return StatusChangeOne::getResultPlayerMessage(
                    (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
                    ? &StatusChange::statusTimeStop_
                    : &self->status_[static_cast<int>(actionType)]);
        }
    }

    if (characterType == dq5::level::CharacterType::MONSTER) {
        switch (relativeIndex) {
        case 0:
            return StatusChangeOne::getResultMonsterMessage(&StatusChange::statusFizzleZone_);
        case 2:
            return StatusChangeOne::getResultMonsterMessage(&StatusChange::statusToherosu_);
        case 3:
            return StatusChangeOne::getResultMonsterMessage(&StatusChange::statusSinobiasi_);
        case 6:
            return StatusChangeOne::getResultMonsterMessage(&StatusChange::statusMonstersFood_);
        default:
            if (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR)
                return StatusChangeOne::getResultMonsterMessage(&StatusChange::statusCloseDoor_);
            else
                return StatusChangeOne::getResultMonsterMessage(
                    (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
                    ? &StatusChange::statusTimeStop_
                    : &self->status_[static_cast<int>(actionType)]);
        }
    }

    return 0;
}


int status::StatusChange::getResultMessage2(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE actionType, dq5::level::CharacterType characterType) {
    // Détermination du pointeur vers le slot de statut
    status::StatusChangeOne* statusPtr = nullptr;

    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusPtr = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        // Ces cas sont traités comme spéciaux ci-dessous
        break;
    default:
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
        break;
    }

    // Cas particulier : pour TIMESTOP, on ne veut pas le global dans certains cas
    if (!statusPtr) {
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
    }

    // Sélection de la méthode à appeler selon le type de personnage
    switch (characterType) {
    case dq5::level::CharacterType::PLAYER:
        return status::StatusChangeOne::getResultPlayerMessage(statusPtr);
    case dq5::level::CharacterType::MONSTER:
        return status::StatusChangeOne::getResultMonsterMessage(statusPtr);
    default:
        return 0;
    }
}


int status::StatusChange::getAgainDisableMessage(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE actionType) {
    status::StatusChangeOne* statusPtr = nullptr;

    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusPtr = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        // spécial, voir plus bas
        break;
    default:
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
        break;
    }

    if (!statusPtr) {
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
    }

    return  status::StatusChangeOne::getAgainDisableMessage(statusPtr);
}


int status::StatusChange::getAgainEnableMessage(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE actionType) {
    status::StatusChangeOne* statusPtr = nullptr;

    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusPtr = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        // géré plus bas
        break;
    default:
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
        break;
    }

    if (!statusPtr) {
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
    }

    return status::StatusChangeOne::getAgainEnableMessage(statusPtr);
}


void status::StatusChange::setTurn(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE actionType,uint8_t turn) {
    status::StatusChangeOne* statusPtr = nullptr;

    switch (actionType) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        statusPtr = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
      
        break;
    default:
        statusPtr = &self->status_[static_cast<int>(actionType)];
        break;
    }

    if (!statusPtr) {
        statusPtr = (actionType == dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ? &status::StatusChange::statusTimeStop_
            : &self->status_[static_cast<int>(actionType)];
    }

    statusPtr->turn_ = turn;
}

void status::StatusChange::printAll(StatusChange* self)
{
    
}

void status::StatusChange::setCount(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE status,uint8_t count)
{
    status::StatusChangeOne* ptr = &status::StatusChange::statusFizzleZone_;

    switch (status) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        ptr = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        ptr = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        ptr = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        ptr = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        ptr = &status::StatusChange::statusTimeStop_;
        if (status != dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP)
            ptr = &self->status_[static_cast<int>(status)];
        break;
    default:
        ptr = &self->status_[static_cast<int>(status)];
        break;
    }

    ptr->count_ = count;
}


void status::StatusChange::execEndOfRoundForAll(StatusChange* self)
{
    for (int i = 51; i >= 0; --i)
    {
        if (i == 9)
        {
            status::StatusChangeOne* timeStop = &status::StatusChange::statusTimeStop_;

            status::StatusChangeOne::execEndOfRound(timeStop);

            if (status::StatusChangeOne::isCancel(timeStop))
            {
                status::StatusChangeOne::setEnable(timeStop, false);
            }
        }
    }
}


void status::StatusChange::execEndOfBattle(StatusChange* self)
{
    for (int i = 0; i < 51; ++i)
    {
        int index = i & 0x7FFFFFFF;
        int offset = index - 33;

        // Statuts spéciaux gérés globalement (ex: indices hors plage, ou bits non autorisés)
        if (offset < 0 || offset > 0xE || ((1 << offset) & 0x7C03) != 0)
        {
            // Rien à faire pour ces cas
            continue;
        }

        status::StatusChange* target = self;

        switch (index)
        {
        case 0x23:  // 35
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusCloseDoor_);
            break;
        case 41: // 0x29
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusFizzleZone_);
            break;
        case 42:
            target = reinterpret_cast<status::StatusChange*>(&status::StatusChange::statusTimeStop_);
            break;
        default:
            break;
        }

        status::StatusChangeOne::clear(target->status_);

        // Avance dans les statuts internes (12 bytes par entrée)
        self = reinterpret_cast<status::StatusChange*>(reinterpret_cast<char*>(self) + 12);
    }

    // Nettoyage explicite global, indépendant de la boucle
    status::StatusChangeOne::clear(&status::StatusChange::statusFizzleZone_);
    status::StatusChangeOne::clear(&status::StatusChange::statusCloseDoor_);
}


void status::StatusChange::StaticClear()
{
    status::StatusChangeOne::clear(&status::StatusChange::statusCloseDoor_);
}


int status::StatusChange::getRestTurn(StatusChange* self,dq5::level::ActionParam::ACTIONTYPE actionType)
{
    status::StatusChangeOne* statusPtr = &status::StatusChange::statusFizzleZone_;

    switch (actionType)
    {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        statusPtr = &status::StatusChange::statusToherosu_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        statusPtr = &status::StatusChange::statusSinobiasi_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        statusPtr = &status::StatusChange::statusMonstersFood_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        statusPtr = &status::StatusChange::statusCloseDoor_;
        break;

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        statusPtr = &status::StatusChange::statusTimeStop_;
        break;

    default:
        statusPtr = &self->status_[static_cast<int>(actionType)];
        break;
    }

    return static_cast<int>(statusPtr->turn_) - static_cast<int>(statusPtr->count_);
}


int status::StatusChange::getToherosInfo()
{
    if (status::StatusChangeOne::isEnable(&status::StatusChange::statusToherosu_))
    {
        uint8_t lowBits = ToherosCount_;
        uint16_t highBits = ToherosTurn_;

        return static_cast<uint16_t>(lowBits + (highBits << 7));
    }

    return 0;
}


int status::StatusChange::getSinobiasiInfo()
{
    if (status::StatusChangeOne::isEnable(&status::StatusChange::statusToherosu_))
    {
        uint8_t lowBits = SinobiasCount_;
        uint16_t highBits = SinobiasTurn_;

        return static_cast<uint16_t>(lowBits + (highBits << 7));
    }

    return 0;
}


int status::StatusChange::getMonstersFoodInfo()
{
    if (status::StatusChangeOne::isEnable(&status::StatusChange::statusToherosu_))
    {
        uint8_t lowBits = MonstersFoodCount_;
        uint16_t highBits = MonstersFoodTurn_;

        return static_cast<uint16_t>(lowBits + (highBits << 7));
    }

    return 0;
}


void status::StatusChange::setToherosInfo(uint16_t value)
{
    char v1; 

    if (value)
    {
        v1 = value;
        status::StatusChangeOne::setup(
            (status::StatusChangeOne*)&status::StatusChange::statusToherosu_,
            dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU,
            value >> 7,
            1);
        ToherosCount_ = v1 & 0x7F;
    }
}

void status::StatusChange::setMonstersFoodInfo(uint16_t value)
{
    char v1; 

    if (value)
    {
        v1 = value;
        status::StatusChangeOne::setup(
            (status::StatusChangeOne*)&status::StatusChange::statusMonstersFood_,
            dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA,
            value >> 7,
            1);
        MonstersFoodCount_ = v1 & 0x7F;
    }
}


status::StatusChangeOne* status::StatusChange::getStatusChangeOne(StatusChange* self, int rel)
{
    switch (rel)
    {
    case 41:
        return &status::StatusChange::statusFizzleZone_;
    case 43: 
        return &status::StatusChange::statusToherosu_;
    case 44: 
        return &status::StatusChange::statusSinobiasi_;
    case 45: 
    case 46: 
    case 42: 
        break; 
    case 47:
        return &status::StatusChange::statusMonstersFood_;
    case 35:
        return &status::StatusChange::statusCloseDoor_;
    default:
        break;
    }

    if (rel == 42) 
        return &status::StatusChange::statusTimeStop_;
    else
        return &self->status_[rel];
}

status::StatusChangeOne* status::StatusChange::getStatusChangeOneForOnlyOne(StatusChange* self,int rel)
{
    status::StatusChangeOne* result; 
    result = &status::StatusChange::statusTimeStop_;
    if (rel != 42)
        return 0;
    return result;
}

void status::StatusChange::cleanupCloseDoor()
{
    status::StatusChangeOne::cleanup(&status::StatusChange::statusCloseDoor_);
}

void status::StatusChange::cleanupFizzleZone()
{
    status::StatusChangeOne::cleanup(&status::StatusChange::statusFizzleZone_);
}

void status::StatusChange::copy(status::StatusChange* self, status::StatusChange* other) {
    for (int i = 0; i < 51; ++i) {
        status::StatusChangeOne* src = nullptr;

        switch (i) {
        case 41:
            src = &status::StatusChange::statusFizzleZone_;
            break;
        case 42:
            src = &status::StatusChange::statusTimeStop_;
            break;
        case 43:
            src = &status::StatusChange::statusToherosu_;
            break;
        case 44:
            src = &status::StatusChange::statusSinobiasi_;
            break;
        case 47:
            src = &status::StatusChange::statusMonstersFood_;
            break;
        case 35:
            src = &status::StatusChange::statusCloseDoor_;
            break;
        default:
            src = &other->status_[i];
            break;
        }

        status::StatusChangeOne* dst = &self->status_[i];

        // Copie fidèle au binaire : flag, actionIndex, turn, count
        dst->flag_.setValue(src->flag_.get());
        dst->actionIndex_ = src->actionIndex_;
        dst->turn_ = src->turn_;
        dst->count_ = src->count_;
    }
}

void status::StatusChange::setupCloseDoor()
{
    status::StatusChangeOne::setup(
        (status::StatusChangeOne*)&status::StatusChange::statusCloseDoor_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR,
        1,
        1);
}


void status::StatusChange::setupFizzleZone()
{
    status::StatusChangeOne::setup(
        (status::StatusChangeOne*)&status::StatusChange::statusFizzleZone_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE,
        1,
        1);
}


void status::StatusChange::print(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel, uint8_t* name)
{

}

dq5::level::ActionParam::ACTIONTYPE status::StatusChange::isRelease(status::StatusChange* self) {
    using namespace dq5::level;

    for (int i = 0; i < static_cast<int>(dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MAX); ++i) {
        dq5::level::ActionParam::ACTIONTYPE type = static_cast<dq5::level::ActionParam::ACTIONTYPE>(i);

        status::StatusChangeOne* entry = nullptr;

        switch (type) {
        case ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
            entry = &status::StatusChange::statusFizzleZone_;
            break;
        case ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
            entry = &status::StatusChange::statusToherosu_;
            break;
        case ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
            entry = &status::StatusChange::statusSinobiasi_;
            break;
        case ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
            entry = &status::StatusChange::statusMonstersFood_;
            break;
        case ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
            entry = &status::StatusChange::statusCloseDoor_;
            break;
        case ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
        case ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
        case ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
            entry = &status::StatusChange::statusTimeStop_;
            break;
        default:
            entry = &self->status_[i];
            break;
        }

        if (status::StatusChangeOne::isRelease(entry)) {
            return type;
        }
    }

    return ActionParam::ACTIONTYPE::ACTIONTYPE_NONE;
}


bool status::StatusChange::isEnableFizzleZone()
{
    return status::StatusChangeOne::isEnable(&status::StatusChange::statusFizzleZone_);
}

bool status::StatusChange::isEnableCloseDoor()
{
    return status::StatusChangeOne::isEnable(&StatusChange::statusCloseDoor_);
}

bool status::StatusChange::isCancel(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* entry = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        entry = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        entry = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        entry = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        entry = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        entry = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        entry = &status::StatusChange::statusTimeStop_;
        break;
    default:
        entry = &self->status_[static_cast<int>(rel)];
        break;
    }

    return status::StatusChangeOne::isCancel(entry);
}


int status::StatusChange::getTurn(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel) {
    status::StatusChangeOne* entry = nullptr;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        entry = &status::StatusChange::statusFizzleZone_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        entry = &status::StatusChange::statusToherosu_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        entry = &status::StatusChange::statusSinobiasi_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        entry = &status::StatusChange::statusMonstersFood_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        entry = &status::StatusChange::statusCloseDoor_;
        break;
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        entry = &status::StatusChange::statusTimeStop_;
        break;
    default:
        entry = &self->status_[static_cast<int>(rel)];
        break;
    }

    return entry->turn_;
}
