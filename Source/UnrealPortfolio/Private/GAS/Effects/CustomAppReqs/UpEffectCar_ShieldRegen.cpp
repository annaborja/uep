// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_ShieldRegen.h"

#include "Characters/UpCharacter.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"

bool UUpEffectCar_ShieldRegen::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
                                                                     const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (const auto Character = Cast<AUpCharacter>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Character->GetVitalAttributeSet())
		{
			return AttributeSet->GetShieldAttribute().GetNumericValue(AttributeSet) <
				AttributeSet->GetMaxShieldAttribute().GetNumericValue(AttributeSet);
		}
	}
	
	return false;
}
