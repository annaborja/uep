// Copyright AB. All Rights Reserved.

#include "GAS/Effects/UpMmc_GunMagazineCapacity.h"

#include "Items/UpWeapon.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

float UUpMmc_GunMagazineCapacity::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto EffectContextHandle = Spec.GetContext();

	if (const auto Weapon = Cast<AUpWeapon>(EffectContextHandle.GetEffectCauser()))
	{
		if (const auto WeaponData = Weapon->GetWeaponData(); WeaponData.IsValid())
		{
			return WeaponData.BaseMagazineCapacity;
		}
	}
	
	return 0.f;
}
