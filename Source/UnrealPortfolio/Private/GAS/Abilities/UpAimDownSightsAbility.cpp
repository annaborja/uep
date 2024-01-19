// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpAimDownSightsAbility.h"

#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Tags/GasTags.h"

UUpAimDownSightsAbility::UUpAimDownSightsAbility()
{
	AbilityTags.AddTag(TAG_Ability_AimDownSights);
	
	ActivationOwnedTags.AddTag(TAG_State_Aiming);
	
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Sprint);
}

void UUpAimDownSightsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(GetAvatarActorFromActorInfo()); PlayableCharacter && PlayableCharacter->IsPlayer())
	{
		if (const auto CustomPlayerController = PlayableCharacter->GetCustomPlayerController())
		{
			CustomPlayerController->SetTargetCameraFov(PlayableCharacter->GetBaseCameraFov() * FovMultiplier);
		}
	}
}

void UUpAimDownSightsAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(GetAvatarActorFromActorInfo()); PlayableCharacter && PlayableCharacter->IsPlayer())
	{
		if (const auto CustomPlayerController = PlayableCharacter->GetCustomPlayerController())
		{
			CustomPlayerController->SetTargetCameraFov(PlayableCharacter->GetBaseCameraFov());
		}
	}
}
