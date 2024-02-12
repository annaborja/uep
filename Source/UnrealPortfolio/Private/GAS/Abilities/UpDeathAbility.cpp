// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpDeathAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Characters/UpCharacter.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Tags/GasTags.h"
#include "Tags/ScriptTags.h"

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
			if (const auto Controller = Cast<AUpAiController>(Character->GetController()))
			{
				if (const auto BrainComponent = Controller->GetBrainComponent())
				{
					BrainComponent->StopLogic(TEXT("Death ability"));
				}
			}
			
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

				if (WaitGameplayEventTask) WaitGameplayEventTask->EndTask();
				
				WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_Death_DetachItems);
				WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
				WaitGameplayEventTask->Activate();
			}
		}
	}
}

void UUpDeathAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
		{
			Character->UnequipItem(EquipmentSlot);
		}

		Character->SetPosture(EUpCharacterPosture::Casual);
	}
	
	WaitGameplayEventTask->EndTask();
}

void UUpDeathAbility::OnMontageCompleted()
{
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s death montage completed"), *GetAvatarActorFromActorInfo()->GetName())
	}
	
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->Die();
	}
}

FName UUpDeathAbility::GetMontageSectionName() const
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo())) {
		if (const auto Montage = Character->GetDeathsMontage())
		{
			return Montage->GetSectionName(FMath::RandRange(0, Montage->GetNumSections() - 1));
		}
	}

	return NAME_None;
}
