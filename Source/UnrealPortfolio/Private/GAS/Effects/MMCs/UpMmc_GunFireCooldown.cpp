// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_GunFireCooldown.h"

#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"

float UUpMmc_GunFireCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto EffectContextHandle = Spec.GetContext();

	if (const auto Character = Cast<AUpCharacter>(EffectContextHandle.GetEffectCauser()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			return Weapon->GetBurstInterval();
		}
	}
	
	return 1.f;
}
