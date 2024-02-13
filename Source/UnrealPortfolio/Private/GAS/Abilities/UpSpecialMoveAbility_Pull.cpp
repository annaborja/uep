// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSpecialMoveAbility_Pull.h"

#include "Tags/GasTags.h"

UUpSpecialMoveAbility_Pull::UUpSpecialMoveAbility_Pull()
{
	AbilityTags.AddTag(TAG_Ability_SpecialMove_Pull);
}

void UUpSpecialMoveAbility_Pull::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
