// Copyright AB. All Rights Reserved.

#include "GAS/Effects/UpMmc_GunMaxAmmoReserve.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

float UUpMmc_GunMaxAmmoReserve::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto EffectContextHandle = Spec.GetContext();

	if (const auto Weapon = Cast<AUpWeapon>(EffectContextHandle.GetEffectCauser()))
	{
		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			return AttributeSet->GetMaxAmmo() - AttributeSet->GetMagazineCapacity();
		}
	}
	
	return 0.f;
}
