#pragma once
#include "status/UseActionParam.h"


namespace status {
	struct ActionEffectValue {
		static void setEffectValue(UseActionParam* useActionParam, bool flag);
		static void checkMultiAttack(UseActionParam* useActionParam);
		static void setEffectValueNormalAttack(status::UseActionParam* useActionParam);
		static bool specialTargetEffectValue_;
		static void setEffectValueBasic(status::UseActionParam* useActionParam);
		static void setEffectValueException(status::UseActionParam* useActionParam);
		static void setEffectValueWithDoubleAttack(status::UseActionParam* useActionParam);
	};
	
}