#include "status/getUsuallyAttackEffectValueForAI.h"
#include "iostream"
#include "status/HaveStatusInfo.h"
#include "dq5/CharacterType.h"
#include "ar/rand.h"



int status::getUsuallyAttackEffectValueForAI(status::CharacterStatus* actor,status::CharacterStatus* target)
{
    status::HaveStatusInfo* actorStatus = &actor->haveStatusInfo_;
    int result = 0;

    unsigned int attack = status::HaveStatusInfo::getAttack(
        reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus), 0);

    int defence = status::HaveStatusInfo::getDefence(
        reinterpret_cast<status::HaveStatusInfo_0*>(&target->haveStatusInfo_), 0);

    int attackDifference = attack - (defence >> 1);

    if (attackDifference >= 2) {
        int halfDiff = attackDifference / 2;
        dq5::level::CharacterType targetType = target->characterType_;

        if (targetType == dq5::level::CharacterType::PLAYER) {
            if (halfDiff > static_cast<int>(attack / 12)) {
                if (attackDifference <= 15) {
                    result = halfDiff - 1;
                }
                else {
                    result = 700 * halfDiff / 800;
                }
            }
            else {
                result = ar::rand(attack / 12);
            }
        }
        else if (targetType == dq5::level::CharacterType::MONSTER) {
            if (attackDifference <= 15) {
                result = halfDiff + ar::rand(3) - 1;
            }
            else {
                result = 700 * halfDiff / 800;
            }
        }
    }

    if (status::HaveStatusInfo::isKaishin(
        reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus)))
    {
        int atk = status::HaveStatusInfo::getAttack(
            reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus), 0);
        result = (ar::rand(11) + 64) * atk / 64;
    }

    if (status::HaveStatusInfo::isTsukon1(
        reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus)))
    {
        int atk = status::HaveStatusInfo::getAttack(
            reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus), 0);
        uint64_t tmp = (0x51EB851FULL * (ar::rand(6) + 100) * atk) >> 32;
        result = static_cast<int>((tmp >> 5) + (tmp >> 31));
    }

    if (status::HaveStatusInfo::isTsukon2(
        reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus)))
    {
        return 5 * status::HaveStatusInfo::getAttack(
            reinterpret_cast<status::HaveStatusInfo_0*>(actorStatus), 0);
    }

    return result;
}
