// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_GunHasAmmoReserveSpace.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

bool UUpEffectCar_GunHasAmmoReserveSpace::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
                                                                                const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (const auto Weapon = Cast<AUpWeapon>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			// We need at least one space available in order to add ammo.
			return AttributeSet->GetMagazineFill() + AttributeSet->GetAmmoReserve() < AttributeSet->GetMaxAmmo();
		}
	}
	
	return false;
}
