// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpActivateEquipmentAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Tags/GasTags.h"
#include "Utils/Enums.h"

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
			if (const auto Montage = Character->GetWeaponEquipMontage())
			{
				const auto MontageSectionName = GetMontageSectionName(Character);

				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
				}
				
				const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
								this, NAME_None, Montage, 1.f, MontageSectionName);
				PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
				PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
				PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageInterrupted);
				PlayMontageAndWaitTask->Activate();
				
				return;
			}
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpActivateEquipmentAbility::OnMontageCompleted()
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (Character->GetEquipment().GetEquipmentSlotData(EquipmentSlot).bActivated)
		{
			Character->DeactivateEquipment(EquipmentSlot);
		} else
		{
			Character->ActivateEquipment(EquipmentSlot);
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpActivateEquipmentAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

FName UUpActivateEquipmentAbility::GetMontageSectionName(const AUpCharacter* Character) const
{
	return NAME_None;
}
