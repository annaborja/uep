// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSwitchOutOfSquadMemberAbility.h"

#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Tags/GasTags.h"

UUpSwitchOutOfSquadMemberAbility::UUpSwitchOutOfSquadMemberAbility()
{
	AbilityTags.AddTag(TAG_Ability_SwitchOutOfSquadMember);
	
	BlockAbilitiesWithTag.AddTag(TAG_Ability);

	auto TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Ability_SwitchOutOfSquadMember;
	
	AbilityTriggers.Add(TriggerData);
}

void UUpSwitchOutOfSquadMemberAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);

	check(Sfx_MergeOut);
}

void UUpSwitchOutOfSquadMemberAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpPlayableCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto CustomPlayerController = Character->GetCustomPlayerController())
		{
			if (TriggerEventData)
			{
				if (const auto TargetCharacter = Cast<AUpPlayableCharacter>(TriggerEventData->Target))
				{
					if (const auto TargetAbilitySystemComponent = TargetCharacter->GetAbilitySystemComponent())
					{
						FGameplayTagContainer AbilityTags;
						AbilityTags.AddTag(TAG_Ability_SwitchIntoSquadMember);
		
						if (TargetAbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags))
						{
							if (Sfx_MergeOut) UGameplayStatics::PlaySound2D(Character, Sfx_MergeOut);

							if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(Character, 0))
							{
								CameraManager->StartCameraFade(1.f, CameraFadeEndAlpha, CameraFadeDuration, CameraFadeColor);
							}

							CustomPlayerController->SwitchCharacter(TargetCharacter);
							
							EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
							return;
						}
					}
				}
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}
