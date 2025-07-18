#include "status/CharacterStatus.h"
#include "status/HaveStatusInfo.h"
#include "dq5/CharacterType.h"
#include "status/getUsuallyAttackEffectValue.h"
#include "ar/rand.h"

int status::getUsuallyAttackEffectValue(CharacterStatus* actor, CharacterStatus* target) {
    if (!actor || !target) return 0;

    // Supposons que HaveStatusInfo_0 est compatible avec haveStatusInfo_
    HaveStatusInfo_0* actorInfo = reinterpret_cast<HaveStatusInfo_0*>(&actor->haveStatusInfo_);
    HaveStatusInfo_0* targetInfo = reinterpret_cast<HaveStatusInfo_0*>(&target->haveStatusInfo_);

    int baseAttack = HaveStatusInfo::getAttack(actorInfo, 0);
    int baseDefence = HaveStatusInfo::getDefence(targetInfo, 0);

    int raw = baseAttack - (baseDefence >> 1);
    int result = 0;

    if (raw > 1) {
        int halfRaw = raw / 2;

        // Ajustement pour l'enum class dq5::level::CharacterType
        using CharacterType = dq5::level::CharacterType;

        if (target->characterType_ == CharacterType::PLAYER) {
            if (halfRaw <= static_cast<int>(baseAttack / 12)) {
                result = ar::rand(baseAttack / 12);
            }
            else if (raw > 15) {
                result = (ar::rand(256) * (25 * halfRaw + 100) / 256 + (700 * halfRaw / 8)) / 100;
            }
            else {
                result = halfRaw + ar::rand(3) - 1;
            }
        }
        else if (target->characterType_ == CharacterType::MONSTER) {
            if (raw > 15) {
                result = (ar::rand(256) * (25 * halfRaw + 100) / 256 + (700 * halfRaw / 8)) / 100;
            }
            else {
                result = halfRaw + ar::rand(3) - 1;
            }
        }
    }
    else {
        result = ar::rand(2);
    }

    // Cas spéciaux
    if (HaveStatusInfo::isKaishin(actorInfo)) {
        int atk = baseAttack;
        result = ((ar::rand(11) + 64) * atk) / 64;
    }

    if (HaveStatusInfo::isTsukon1(actorInfo)) {
        int atk = baseAttack;
        // Multiplication 64 bits + shift, version précise
        uint64_t val = (1374389535ULL * (ar::rand(6) + 100) * static_cast<uint64_t>(atk)) >> 32;
        result = static_cast<int>((val >> 5) + (static_cast<uint32_t>(val) >> 31));
    }

    if (HaveStatusInfo::isTsukon2(actorInfo)) {
        return 5 * baseAttack;
    }

    return result;
}
