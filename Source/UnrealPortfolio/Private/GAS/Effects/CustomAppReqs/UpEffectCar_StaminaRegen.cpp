// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_StaminaRegen.h"

#include "AbilitySystemComponent.h"
#include "Characters/UpCharacter.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"

bool UUpEffectCar_StaminaRegen::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
                                                                      const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (const auto Character = Cast<AUpCharacter>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Character->GetVitalAttributeSet())
		{
			return AttributeSet->GetStaminaAttribute().GetNumericValue(AttributeSet) <
				AttributeSet->GetMaxStaminaAttribute().GetNumericValue(AttributeSet);
		}
	}
	
	return false;
}
