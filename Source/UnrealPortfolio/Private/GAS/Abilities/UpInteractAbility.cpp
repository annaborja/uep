// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpInteractAbility.h"

#include "Characters/UpCharacter.h"
#include "Interfaces/UpInteractable.h"
#include "Tags/GasTags.h"

UUpInteractAbility::UUpInteractAbility()
{
	AbilityTags.AddTag(TAG_Ability_Interact);

	auto TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Ability_Interact;
	
	AbilityTriggers.Add(TriggerData);
}

void UUpInteractAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData)
	{
		InteractableActor = TriggerEventData->OptionalObject;

		if (const auto Interactable = Cast<IUpInteractable>(InteractableActor))
		{
			if (const auto AvatarActor = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
			{
				if (const auto Controller = AvatarActor->GetInstigatorController())
				{
					if (const_cast<IUpInteractable*>(Interactable)->Interact(Controller))
					{
						EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
					}
					
					return;
				}
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UUpInteractAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (const auto Interactable = Cast<IUpInteractable>(InteractableActor))
	{
		if (const auto AvatarActor = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (const auto Controller = AvatarActor->GetInstigatorController())
			{
				const_cast<IUpInteractable*>(Interactable)->OnInteractionEnd(Controller);
			}
		}
	}
	
	InteractableActor = nullptr;
}
