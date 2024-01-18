// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UUpAttackAbility::TriggerDamage(AActor* TargetActor) const
{
	if (const auto SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		SourceAbilitySystemComponent && DamageEffectClass)
	{
		if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext()).Data.Get(),
				TargetAbilitySystemComponent);
		}
	}
}
