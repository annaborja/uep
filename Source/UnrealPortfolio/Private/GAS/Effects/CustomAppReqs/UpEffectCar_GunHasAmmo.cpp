// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_GunHasAmmo.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

bool UUpEffectCar_GunHasAmmo::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
																	const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (const auto Weapon = Cast<AUpWeapon>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			return AttributeSet->GetAmmoReserveAttribute().GetNumericValue(AttributeSet) >= 1.f;
		}
	}
	
	return false;
}
