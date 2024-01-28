// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSpecialMoveAbility_Push.h"

#include "Tags/GasTags.h"

UUpSpecialMoveAbility_Push::UUpSpecialMoveAbility_Push()
{
	AbilityTags.AddTag(TAG_Ability_SpecialMove_Push);
}

void UUpSpecialMoveAbility_Push::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
