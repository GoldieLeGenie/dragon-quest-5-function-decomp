#include "status/HaveAction.h"
#include "status/BaseHaveAction.h"
#include "status/PlayerData.h"
#include "status/HaveAction.h"
#include "status/HaveStatus.h"
#include "menu/TownMenuItemSelectTargetChara.h"
#include "dq5/PlayerData1.h"
#include "iostream"
#include <cstring>  



int status::HaveAction::mode_ = 0;
int status::HaveAction::addRulaLevel_ = 0;         
int status::HaveAction::addParupunteLevel_ = 0;         



bool status::HaveAction::isBattleMode() {
        return status::HaveAction::mode_ == 1;
    }

void status::HaveAction::setBattleMode() {
    status::HaveAction::mode_ = 1;
}

status::HaveAction::HaveAction()
{
    for (int i = 0; i != 18; ++i)
        status::BaseHaveAction::BaseHaveAction(this->action_[i]);
    for (int j = 0; j != 18; ++j)
        status::BaseHaveAction::clear(&this->action_[j]);
    this->level_ = 0;
}

void status::HaveAction::sort(HaveAction* self, int index) {
    if (index < 0 || index >= 16)
        return;

    for (int i = index; i < 16; ++i) {
        self->action_[i].actionLevel_ = self->action_[i + 1].actionLevel_;
        self->action_[i].battleAction_ = self->action_[i + 1].battleAction_;
    }
}

void status::HaveAction::clear(HaveAction* self)
{
    for (int i = 0; i != 18; ++i)
        status::BaseHaveAction::clear(&self->action_[i]);
    self->level_ = 0;
}

void status::HaveAction::setup(HaveAction* self, int index, int level)
{
    // On vide d'abord les actions existantes
    for (int i = 0; i < 18; ++i) {
        status::BaseHaveAction::clear(&self->action_[i]);
    }

    // Récupère un pointeur sur le PlayerData correspondant à l'index
    status::PlayerData* player = &status::playerData_[index];

    self->level_ = 0;

    std::memcpy(self, reinterpret_cast<char*>(player) + 0x78, sizeof(status::HaveAction));

    self->index_ = index;
    self->kind_ = player->kindIndex_;
}

void status::HaveAction::add(HaveAction* self, int battleAction, int usuallyAction, bool flag) {
    bool CurrenFlag = flag;

    if (usuallyAction == 129 && self->index_ == 1) {
        battleAction = reinterpret_cast<int>(&menu::TownMenuItemSelectTargetChara::menuDraw);
        CurrenFlag = true;

        if (status::HaveAction::addRulaLevel_ == self->level_) {
            status::HaveAction::addRulaLevel_ = self->level_;
        }
    }

    if (battleAction == 66 && self->index_ == 1) {
        CurrenFlag = true;
        status::HaveAction::addParupunteLevel_ = self->level_; 
    }


    for (int i = 0; i < 18; ++i) {
        if (!status::BaseHaveAction::isEnable(&self->action_[i])) {
            status::BaseHaveAction::add(&self->action_[i], battleAction, usuallyAction);
            if (!CurrenFlag)
                status::BaseHaveAction::remembering(&self->action_[i]);
            break;
        }
    }
}

void status::HaveAction::clearRememberingAction(HaveAction* self)
{

    for (int i = 0; i != 18; ++i)
    {
        if (status::BaseHaveAction::isRemembering(&self->action_[i]))
            status::BaseHaveAction::clearRemembering(&self->action_[i]);
    }
}


int status::HaveAction::getRememberingAction(status::HaveAction* self)
{
    for (int i = 0; i < 18; ++i)
    {
        status::BaseHaveAction* act = &self->action_[i];

        if (status::BaseHaveAction::isRemembering(act))
        {
            status::BaseHaveAction::clearRemembering(act);

            int action = status::BaseHaveAction::getAction(act);
            if (action)
                return action;

            int usual = status::BaseHaveAction::getUsuallyAction(act);
            if (usual)
                return usual;
        }
    }

    return 0;
}


bool status::HaveAction::isHaveAction(HaveAction* self, int actionIndex)
{
    uint32_t enabledCount = 0;
    for (int i = 0; i < 18; ++i)
    {
        if (status::BaseHaveAction::isEnable(&self->action_[i]))
            ++enabledCount;
    }

    for (unsigned int i = 0; i < enabledCount; ++i)
    {
        int currentAction = status::HaveAction::getHaveAction(self, i);
        if (currentAction == actionIndex)
            return true;
    }

    return false;
}

int status::HaveAction::getHaveAction(HaveAction* self, int index)
{
    return status::HaveAction::getAction(self, index);
}

int status::HaveAction::getAction(HaveAction* self, int index)
{
    if (status::HaveAction::mode_)
    {
        int validCount = 0;
        for (int i = 0; i < 18; ++i)
        {
            int action = status::BaseHaveAction::getAction(&self->action_[i]);
            if (action)
            {
                if (validCount == index)
                    return action;
                ++validCount;
            }
        }
    }
    else
    {
        int validCount = 0;
        for (int i = 0; i < 18; ++i)
        {
            int usualAction = status::BaseHaveAction::getUsuallyAction(&self->action_[i]);
            if (!usualAction)
                continue;

            if (validCount == index)
                return usualAction;
            ++validCount;
        }
    }

    return 0;
}

bool status::HaveAction::isAction(HaveAction* self, int actionIndex)
{
    int count = status::HaveAction::getCount(self);
    if (count < 1)
        return false;

    for (int i = 0; i < count; ++i)
    {
        if (status::HaveAction::getHaveAction(self, i) == actionIndex)
            return true;
    }

    return false;
}


int status::HaveAction::getCount(HaveAction* self)
{
    int count = 0;

    for (int i = 0; i < 18; ++i)
    {
        if (status::BaseHaveAction::isRemember(&self->action_[i]))
        {
            if (!status::HaveAction::mode_)
            {
                if (status::BaseHaveAction::getUsuallyAction(&self->action_[i]))
                    ++count;
            }
            else if (status::BaseHaveAction::getAction(&self->action_[i]))
            {
                ++count;
            }
        }
    }

    return count;
}


int status::HaveAction::getActionAll(HaveAction* self, int index)
{
    for (int i = 0; i < 18; ++i)
    {
        if (status::BaseHaveAction::getUsuallyAction(&self->action_[i]))
        {
            if (index == i)
                return status::BaseHaveAction::getUsuallyAction(&self->action_[i]);
        }
        else
        {
            if (index == i)
                return status::BaseHaveAction::getAction(&self->action_[i]);
        }
    }
    return 0;
}

int status::HaveAction::getCountAll(HaveAction* self)
{
    int count = 0;
    for (int i = 0; i < 18; ++i)
    {
        if (status::BaseHaveAction::isEnable(&self->action_[i]))
        {
            ++count;
        }
    }
    return count;
}


int status::HaveAction::getBattleAction(HaveAction* self, int index)
{
    int foundCount = 0;

    for (int i = 0; i < 18; ++i)
    {
        int action = status::BaseHaveAction::getAction(&self->action_[i]);
        if (action == 0)
            continue;

        if (foundCount == index)
            return action;

        ++foundCount;
    }

    return 0;
}

int status::HaveAction::getTownAction(HaveAction* self, int index)
{
    int foundCount = 0;

    for (int i = 0; i < 18; ++i)
    {
        int usualAction = status::BaseHaveAction::getUsuallyAction(&self->action_[i]);
        if (usualAction == 0)
            continue;

        if (foundCount == index)
            return usualAction;

        ++foundCount;
    }

    return 0;
}


void status::HaveAction::del(status::HaveAction* self, int index)
{
    status::BaseHaveAction::del(&self->action_[index]);

    if (index <= 16)
    {
        for (int i = index; i < 16; ++i)
        {
            self->action_[i] = self->action_[i + 1];
        }
        status::BaseHaveAction::del(&self->action_[16]);
    }
}


void status::HaveAction::levelup(HaveAction* self, int index, int level, bool flag)
{
    status::HaveAction::execLevelup(self, level, 0);
}


void status::HaveAction::execLevelup(HaveAction* self, int level, bool flag)
{
    int currentLevel = self->level_;
    if (currentLevel == level)
        return;

    // On limite la baisse à -100 (semble être un clamp spécifique au système)
    int startLevel = -2 - currentLevel;
    if (startLevel <= -100)
        startLevel = -100;
    int PlayerLvl = -2 - startLevel; // ça revient à currentLevel (à vérifier)

    while (PlayerLvl < level)
    {
        ++PlayerLvl;

        dq5::level::PlayerData1* levelupData = status::HaveStatus::getLevelupData(self->kind_, PlayerLvl);
        int normalAction1 = levelupData->normalAction1;
        int battleAction1 = levelupData->battleAction1;

        if (normalAction1 != 0 || battleAction1 != 0)
        {
            self->level_ = PlayerLvl;

            // Ajout de l'action principale
            status::HaveAction::add(self, battleAction1, normalAction1, flag);

            // Ajout des actions secondaires si elles existent
            if (levelupData->normalAction2 != 0 || levelupData->battleAction2 != 0)
                status::HaveAction::add(self, levelupData->battleAction2, levelupData->normalAction2, flag);

            if (levelupData->normalAction3 != 0 || levelupData->battleAction3 != 0)
                status::HaveAction::add(self, levelupData->battleAction3, levelupData->normalAction3, flag);

            if (levelupData->normalAction4 != 0 || levelupData->battleAction4 != 0)
                status::HaveAction::add(self, levelupData->battleAction4, levelupData->normalAction4, flag);

            // Cas particulier sur index_ == 1 avec vérifications spécifiques
            if (self->index_ == 1)
            {
                int lvl = self->level_;
                if (status::HaveAction::addRulaLevel_ == lvl)
                    status::HaveAction::add(self, 0, 129, false);

                if (status::HaveAction::addParupunteLevel_ == lvl)
                    status::HaveAction::add(self, 66, 0, false);
            }
        }
    }
}


void status::HaveAction::loadLevelup(HaveAction* self, int level, int rulaLevel, int parupunteLevel)
{
    for (int i = 0; i < 18; ++i)
    {
        status::BaseHaveAction::clear(&self->action_[i]);
    }
    self->level_ = 0;
    status::HaveAction::addParupunteLevel_ = parupunteLevel;
    status::HaveAction::addRulaLevel_ = rulaLevel;
    status::HaveAction::execLevelup(self, level, true);
    status::HaveAction::clearRememberingAction(self);
}


void status::HaveAction::debugLevelup(HaveAction* self, int level)
{
    status::HaveAction::execLevelup(self, level, 1);
}


void status::HaveAction::cleanup(status::HaveAction* self, int index)
{
    char* dest = reinterpret_cast<char*>(&status::PlayerDataAll::playerData_) + 276 * index + 120;
    memcpy(dest, self, 114);
}

void status::HaveAction::setActionMode(status::HaveAction::ActionMode md)
{
    status::HaveAction::mode_ = md;
}

int status::HaveAction::getActionMode()
{
    return status::HaveAction::mode_;
}

bool status::HaveAction::isTownMode()
{
    return status::HaveAction::mode_ == 0;
}

void status::HaveAction::setTownMode()
{
    status::HaveAction::mode_ = 0;
}

status::HaveAction::~HaveAction() {
    for (int i = 17; i >= 0; --i) {
        this->action_[i].~BaseHaveAction();
    }
}