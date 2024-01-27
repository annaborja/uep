// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpActivateEquipmentAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Tags/GasTags.h"
#include "Tags/ScriptTags.h"
#include "Utils/Constants.h"
#include "Utils/Enums.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpActivateEquipmentAbility::UUpActivateEquipmentAbility()
{
	AbilityTags.AddTag(TAG_Ability_ActivateEquipment);

	auto TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Ability_ActivateEquipment;
	
	AbilityTriggers.Add(TriggerData);
}

void UUpActivateEquipmentAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	NumMontagesRemaining = 1;
	NumGameplayEventsRemaining = NumMontagesRemaining;

	if (TriggerEventData)
	{
		MainEquipmentSlot = static_cast<EUpEquipmentSlot::Type>(TriggerEventData->EventMagnitude);
		RelatedEquipmentSlot = MainEquipmentSlot;
		
		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Equipment slot: %d"), MainEquipmentSlot)
		}

		if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
		{
			const auto& Equipment = Character->GetEquipment();

			bActivate = !Equipment.GetEquipmentSlotData(MainEquipmentSlot).bActivated;

			if (const auto PotentialActiveWeaponSlot = Equipment.GetPotentialActiveWeaponSlot();
				bActivate && Equipment.GetEquipmentSlotData(PotentialActiveWeaponSlot).bActivated)
			{
				RelatedEquipmentSlot = PotentialActiveWeaponSlot;
				NumMontagesRemaining = 2;
				NumGameplayEventsRemaining = NumMontagesRemaining;
			}
			
			PlayMontage(Character);
			
			return;
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UUpActivateEquipmentAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{
	const auto bRelatedGameplayEvent = NumGameplayEventsRemaining > 1;
	
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Item = Character->GetEquipment().GetEquipmentSlotData(
			bRelatedGameplayEvent ? RelatedEquipmentSlot : MainEquipmentSlot).ItemInstance.ItemActor)
		{
			if (!bActivate || bRelatedGameplayEvent)
			{
				Character->AttachDeactivatedItem(Item);
			} else
			{
				Character->AttachActivatedItem(Item);
			}
		}
	}

	if (bRelatedGameplayEvent)
	{
		RelatedWaitGameplayEventTask->EndTask();
	} else
	{
		MainWaitGameplayEventTask->EndTask();
	}

	NumGameplayEventsRemaining--;
}

void UUpActivateEquipmentAbility::OnMontageCompleted()
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		const auto bRelatedMontage = NumMontagesRemaining > 1;
		const auto EquipmentSlot = bRelatedMontage ? RelatedEquipmentSlot : MainEquipmentSlot;

		if (!bActivate || bRelatedMontage)
		{
			Character->DeactivateEquipment(EquipmentSlot);
		} else
		{
			Character->ActivateEquipment(EquipmentSlot);
		}
		
		NumMontagesRemaining--;

		if (NumMontagesRemaining > 0)
		{
			PlayMontage(Character);
			return;
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpActivateEquipmentAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

FName UUpActivateEquipmentAbility::GetMontageSectionName(const AUpCharacter* Character, const EUpEquipmentSlot::Type EquipmentSlot) const
{
	FString MontageSectionString = TEXT("");

	if (const auto Weapon = Cast<AUpWeapon>(Character->GetEquipment().GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor))
	{
		if (Weapon->IsPistolType())
		{
			MontageSectionString += NAME_STRING_PISTOL_TYPE;
		} else if (Weapon->IsRifleType())
		{
			MontageSectionString += NAME_STRING_RIFLE_TYPE;
		}
	}
	
	MontageSectionString += TEXT(".");
	MontageSectionString += bActivate ? TEXT("Activate") : TEXT("Deactivate");

	return FName(MontageSectionString);
}

void UUpActivateEquipmentAbility::PlayMontage(const AUpCharacter* Character)
{
	if (const auto Montage = Character->GetWeaponEquipMontage())
	{
		const auto bRelatedMontage = NumMontagesRemaining > 1;
		const auto MontageSectionName = GetMontageSectionName(Character, bRelatedMontage ? RelatedEquipmentSlot : MainEquipmentSlot);

		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
		}

		if (Montage->IsValidSectionName(MontageSectionName))
		{
			const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this, NAME_None, Montage, 1.f, MontageSectionName);
			PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageCompleted);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageInterrupted);
			PlayMontageAndWaitTask->Activate();

			const auto WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this, TAG_Event_ActivateEquipment_SocketSwitch);
			WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
			WaitGameplayEventTask->Activate();

			if (bRelatedMontage)
			{
				if (RelatedWaitGameplayEventTask) RelatedWaitGameplayEventTask->EndTask();

				RelatedWaitGameplayEventTask = WaitGameplayEventTask;
			} else
			{
				if (MainWaitGameplayEventTask) MainWaitGameplayEventTask->EndTask();

				MainWaitGameplayEventTask = WaitGameplayEventTask;
			}
			
			return;
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}
