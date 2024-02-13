// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSpecialMoveAbility_Aoe.h"

#include "Tags/GasTags.h"

UUpSpecialMoveAbility_Aoe::UUpSpecialMoveAbility_Aoe()
{
	AbilityTags.AddTag(TAG_Ability_SpecialMove_Aoe);
}

void UUpSpecialMoveAbility_Aoe::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
