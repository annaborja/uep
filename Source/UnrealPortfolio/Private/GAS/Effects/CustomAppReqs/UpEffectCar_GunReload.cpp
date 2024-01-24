// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_GunReload.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

bool UUpEffectCar_GunReload::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
																	const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (const auto Weapon = Cast<AUpWeapon>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			// Requirements:
			//   - Has reserve ammo
			//   - Has available magazine capacity
			return AttributeSet->GetAmmoReserve() >= 1.f && AttributeSet->GetMagazineFill() < AttributeSet->GetMagazineCapacity();
		}
	}
	
	return false;
}
