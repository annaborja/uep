// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSprintAbility.h"

#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Tags/GasTags.h"

UUpSprintAbility::UUpSprintAbility()
{
	AbilityTags.AddTag(TAG_Ability_Sprint);
	
	CancelAbilitiesWithTag.AddTag(TAG_Ability_AimDownSights);

	RepeatInterval = 0.1f;
}

void UUpSprintAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);

	check(CostGameplayEffectClass);
}

void UUpSprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
		{
			CustomMovementComponent->ToggleSprint(true);
		}
	}
}

void UUpSprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
		{
			CustomMovementComponent->ToggleSprint(false);
		}
	}
}

void UUpSprintAbility::HandleRepeatAction(const int32 ActionNumber)
{
	Super::HandleRepeatAction(ActionNumber);

	if (!CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
	}
}
