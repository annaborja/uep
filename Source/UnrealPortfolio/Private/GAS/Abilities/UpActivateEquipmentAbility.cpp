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

	if (TriggerEventData)
	{
		EquipmentSlot = static_cast<EUpEquipmentSlot::Type>(TriggerEventData->EventMagnitude);
		
		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Equipment slot: %d"), EquipmentSlot)
		}
		
		if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
		{
			bActivate = !Character->GetEquipment().GetEquipmentSlotData(EquipmentSlot).bActivated;

			if (const auto Montage = Character->GetWeaponEquipMontage())
			{
				const auto MontageSectionName = GetMontageSectionName(Character);

				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
				}
				
				const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
								this, NAME_None, Montage, 1.f, MontageSectionName);
				PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageCompleted);
				PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
				PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageInterrupted);
				PlayMontageAndWaitTask->Activate();

				if (WaitGameplayEventTask) WaitGameplayEventTask->EndTask();
				
				WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
					this, TAG_Event_ActivateEquipment_SocketSwitch);
				WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
				WaitGameplayEventTask->Activate();
				
				return;
			}
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpActivateEquipmentAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Item = Character->GetEquipment().GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
		{
			bActivate ? Character->AttachActivatedItem(Item) : Character->AttachDeactivatedItem(Item);
		}
	}

	WaitGameplayEventTask->EndTask();
}

void UUpActivateEquipmentAbility::OnMontageCompleted()
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		bActivate ? Character->ActivateEquipment(EquipmentSlot) : Character->DeactivateEquipment(EquipmentSlot);
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpActivateEquipmentAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

FName UUpActivateEquipmentAbility::GetMontageSectionName(const AUpCharacter* Character) const
{
	FString MontageSectionString = TEXT("");
	const auto& EquipmentSlotData = Character->GetEquipment().GetEquipmentSlotData(EquipmentSlot);

	if (const auto Weapon = Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor))
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
