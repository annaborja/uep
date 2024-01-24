// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpDeathAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Characters/UpPlayableCharacter.h"
#include "Tags/GasTags.h"

UUpDeathAbility::UUpDeathAbility()
{
	AbilityTags.AddTag(TAG_Ability_Death);
	
	ActivationOwnedTags.AddTag(TAG_State_Dead);

	CancelAbilitiesWithTag.AddTag(TAG_Ability);
	
	BlockAbilitiesWithTag.AddTag(TAG_Ability);
}

void UUpDeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(Character); PlayableCharacter && PlayableCharacter->IsPlayer())
		{
			// TODO(P0): Handle player death.
		} else
		{
			if (const auto Montage = Character->GetDeathsMontage())
			{
				const auto MontageSectionName = GetMontageSectionName();

				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
				}
			
				const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
					this, NAME_None, Montage, 1.f, MontageSectionName);
				PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageCompleted);
				PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCompleted);
				PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCompleted);
				PlayMontageAndWaitTask->Activate();
			}
		}
	}
}

void UUpDeathAbility::OnMontageCompleted()
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->Die();
	}
}

FName UUpDeathAbility::GetMontageSectionName()
{
	return NAME_None;
}
