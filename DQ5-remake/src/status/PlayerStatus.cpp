#include "status/PlayerStatus.h"
#include "status/HaveStatusInfo_0.h"
#include "status/StageAttribute.h"
#include "dq5/StageDamage.h"
#include "dq5/CharacterType.h"


status::PlayerStatus::PlayerStatus() :
    CharacterStatus(),  
    index_(0),
    characterIndex_(-1),  
    walkCount_(0),
    walkCountBarrier_(0),
    walkCountPoison_(0),
    walkCountVenom_(0),
    walkCountLuckHat_(0),
    walkCountLifeRing_(0),
    walkCountCurse4_(0),
    walkCountCurse5_(0)
{
   
}


void status::PlayerStatus::setWalkDamage(PlayerStatus* self, int dmg)
{
    status::HaveStatusInfo* p_haveStatusInfo;
    int Hp;
    int deltaHp;

    p_haveStatusInfo = &self->haveStatusInfo_;
    Hp = status::HaveStatusInfo::getHp((const status::HaveStatusInfo_0*)&self->haveStatusInfo_);

    if (Hp <= dmg)
        deltaHp = 1 - Hp;
    else
        deltaHp = -dmg;

    status::HaveStatusInfo::addHp((status::HaveStatusInfo_0*)p_haveStatusInfo, deltaHp);
}


bool status::PlayerStatus::walkDamage(PlayerStatus* self, dq5::StageDamage damage)
{
    bool result = false;

    if (!status::HaveStatusInfo::isDeath((const status::HaveStatusInfo_0*)&self->haveStatusInfo_))
    {
        if (damage == dq5::StageDamage::Yogan || damage == dq5::StageDamage::Poison)
        {
            if (!status::StageAttribute::isToramana())
            {
                result = true;
                status::PlayerStatus::setWalkDamage(self, 1);
            }
        }
        else if (damage == dq5::StageDamage::Barrier && !status::StageAttribute::isToramana())
        {
            status::PlayerStatus::setWalkDamage(self, 15);
            return true;
        }
    }

    return result;
}


void status::PlayerStatus::walkEnvioronment(PlayerStatus* self)
{
    if (!self->walkCount_ && status::StageAttribute::isEncount((const status::StageAttribute*)&status::g_StageAttribute))
        status::StatusChange::execWalk(&self->haveStatusInfo_.statusChange_);
}

bool status::PlayerStatus::walkNormal(PlayerStatus* self) {
    
    self->walkCount_ += 2;

    if (self->walkCount_ < 16)
        return false;

    self->walkCount_ = 0;

    if (status::HaveStatusInfo::isDeath((const status::HaveStatusInfo_0*)&self->haveStatusInfo_) ||
        !status::StageAttribute::isEncount((const status::StageAttribute*)&status::g_StageAttribute))
    {
        return false;
    }

    bool poisoned = status::PlayerStatus::setWalkEffectPoison(self);
    bool venom = status::PlayerStatus::setWalkEffectVenom(self);
    status::PlayerStatus::setWalkEffectLuckHat(self);
    status::PlayerStatus::setWalkEffectLifeRing(self);

    bool anyStatus = poisoned || venom;

    if (status::HaveEquipment::isEquipment(&self->haveStatusInfo_.haveEquipment_, 271)) {
        status::HaveStatusInfo::addExp((status::HaveStatusInfo_0*)&self->haveStatusInfo_, 1);
    }

    bool cursed4 = status::PlayerStatus::setWalkEffectCurse4(self);
    bool cursed5 = status::PlayerStatus::setWalkEffectCurse5(self);

    return anyStatus || cursed4 || cursed5;
}

bool status::PlayerStatus::setWalkEffectPoison(PlayerStatus* self) {
    bool triggered = false;

    if (status::StatusChange::isEnable(
        &self->haveStatusInfo_.statusChange_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_DOKU))
    {
        short& count = self->walkCountPoison_;

        if (count % 4 == 3) {
            triggered = true;
            status::PlayerStatus::setWalkDamage(self, 1);
        }

        
        ++count;
    }

    return triggered;
}


bool status::PlayerStatus::setWalkEffectVenom(PlayerStatus* self) {
    bool triggered = false;

    if (status::StatusChange::isEnable(
        &self->haveStatusInfo_.statusChange_,
        dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_MOUDOKU))
    {
        short& count = self->walkCountVenom_;

        if (count % 4 == 3) {
            unsigned int hpMax = status::HaveStatus::getHpMax(&self->haveStatusInfo_.haveStatus_);
            triggered = true;

            unsigned int damage = (hpMax > 5) ? (hpMax / 6) : 1;
            status::PlayerStatus::setWalkDamage(self, damage);
        }

        ++count;
    }

    return triggered;
}


void status::PlayerStatus::setWalkEffectLuckHat(PlayerStatus* self)
{
    if (status::HaveEquipment::isEquipment(&self->haveStatusInfo_.haveEquipment_, 147))
    {
        short& count = self->walkCountLuckHat_;
        if (count % 4 == 3)
        {
            status::HaveStatusInfo::addMp((status::HaveStatusInfo_0*)&self->haveStatusInfo_, 1);
        }
        ++count;
    }
    else
    {
        self->walkCountLuckHat_ = 0; 
    }
}

void status::PlayerStatus::setWalkEffectLifeRing(PlayerStatus* self)
{
    if (status::HaveEquipment::isEquipment(&self->haveStatusInfo_.haveEquipment_, 160))
    {
        status::HaveStatusInfo::addHp((status::HaveStatusInfo_0*)&self->haveStatusInfo_, 1);
        ++self->walkCountLifeRing_;
    }
    else
    {
        self->walkCountLifeRing_ = 0;
    }
}

bool status::PlayerStatus::setWalkEffectCurse4(PlayerStatus* self)
{
    status::HaveStatusInfo_0* p_haveStatusInfo;
    bool result = false;
    if (status::StatusChange::isEnable(&self->haveStatusInfo_.statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4))
    {
        if (self->walkCountCurse4_ % 4 == 3)
        {
            p_haveStatusInfo = (status::HaveStatusInfo_0*)&self->haveStatusInfo_;
            if (status::HaveStatusInfo::getHp(p_haveStatusInfo) >= 2)
            {
                status::HaveStatusInfo::addHp(p_haveStatusInfo, -1);
                result = true;
            }
        }
        ++self->walkCountCurse4_;
    }
    return result;
}


bool status::PlayerStatus::setWalkEffectCurse5(PlayerStatus* self)
{
    bool result = false;

    if (status::StatusChange::isEnable(&self->haveStatusInfo_.statusChange_, dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5))
    {
        if (self->walkCountCurse5_ % 4 == 3)
        {
            status::HaveStatusInfo::addMp((status::HaveStatusInfo_0*)&self->haveStatusInfo_, -1);
            result = true;
        }
        ++self->walkCountCurse5_;
    }
    else
    {
        result = false;
    }

    return result;
}


void status::PlayerStatus::setup(PlayerStatus* self, int index)
{
    self->index_ = index;

    if (index >= 16)
        self->characterIndex_ = static_cast<short>((index - 16) / 3 + 16);
    else
        self->characterIndex_ = static_cast<short>(index);

    char* base = reinterpret_cast<char*>(self);

    char* p_characterIndex = base + offsetof(status::PlayerStatus, characterIndex_);

    *reinterpret_cast<int*>(p_characterIndex - 639) = 0;

    status::HaveStatusInfo::setup(
        reinterpret_cast<status::HaveStatusInfo_0*>(p_characterIndex - 637),
        index, 1);

    int charIdxValue = *reinterpret_cast<short*>(p_characterIndex);

    *reinterpret_cast<int*>(p_characterIndex - 601) = reinterpret_cast<int>(p_characterIndex - 107);

    status::HaveBattleStatus::setup(
        reinterpret_cast<status::HaveBattleStatus*>(p_characterIndex - 107),
        dq5::level::CharacterType::PLAYER, 0, charIdxValue);

    *reinterpret_cast<int*>(p_characterIndex - 15) = index;

    *reinterpret_cast<int*>(p_characterIndex - 97) = reinterpret_cast<int>(p_characterIndex - 637);
}



void status::PlayerStatus::levelup(PlayerStatus* self, int level)
{
    uint8_t playerType; 

    playerType = static_cast<uint8_t>(self->haveStatusInfo_.haveStatus_.playerType_);
    if (playerType <= 5 && ((1 << playerType) & 0x26))
    {
        status::HaveStatusInfo::levelup((status::HaveStatusInfo_0*)&self->haveStatusInfo_, level);
    }
}


void status::PlayerStatus::cleanup(PlayerStatus* self)
{
    status::HaveStatusInfo::cleanup((status::HaveStatusInfo_0*)&self->haveStatusInfo_);
}



void status::PlayerStatus::clearWalkDamage(PlayerStatus* self, bool flag)
{
    self->walkCount_ = 0;
    if (flag)
    {
        self->walkCountPoison_ = 0;
        self->walkCountLuckHat_ = 0;
        self->walkCountCurse4_ = 0;
    }
}

void status::PlayerStatus::setBestCondition(PlayerStatus* self)
{
    status::HaveStatus* p_haveStatus; 

    p_haveStatus = &self->haveStatusInfo_.haveStatus_;
    status::HaveStatus::addBaseHp(&self->haveStatusInfo_.haveStatus_, 1000);
    status::HaveStatus::addBaseMp(p_haveStatus, 1000);
}

void status::PlayerStatus::execStartOfRound(PlayerStatus* self)
{
    if (self->haveStatusInfo_.haveStatus_.command_ != 5)
        status::HaveBattleStatus::setSelectCommand(&self->haveBattleStatus_, status::HaveBattleStatus::SelectCommand::AISelect, 0);
}

void status::PlayerStatus::printAction(status::PlayerStatus* self)
{
    
}

void status::PlayerStatus::print(status::PlayerStatus* self)
{
    
}


void status::PlayerStatus::addItem(PlayerStatus* self, int itemIndex)
{
    self->haveStatusInfo_.haveItem_.add(itemIndex);
}

void status::PlayerStatus::delItem(PlayerStatus* self, int index)
{
    self->haveStatusInfo_.haveItem_.del(index);
}
