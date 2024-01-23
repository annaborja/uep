// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_Damage.h"

#include "Interfaces/UpCombatable.h"

float UUpMmc_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto EffectContextHandle = Spec.GetContext();

	if (const auto Combatable = Cast<IUpCombatable>(EffectContextHandle.GetEffectCauser()))
	{
		if (const auto HitResult = EffectContextHandle.GetHitResult())
		{
			return Combatable->CalculateDamageDealt(*HitResult);
		}
	}
	
	return 0.f;
}
