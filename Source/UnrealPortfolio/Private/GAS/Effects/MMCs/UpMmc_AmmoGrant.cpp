// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_AmmoGrant.h"

#include "Items/UpWeapon.h"

float UUpMmc_AmmoGrant::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto& EffectContext = Spec.GetEffectContext();
	
	if (const auto Weapon = Cast<AUpWeapon>(EffectContext.GetSourceObject()))
	{
		return Weapon->GetAmmoGrantAmount();
	}
	
	return 0.f;
}
