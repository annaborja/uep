// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_UpdateEquipment.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "Tags/GasTags.h"
#include "Tags/ScriptTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

EBTNodeResult::Type UUpBtTask_UpdateEquipment::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Character = Cast<AUpCharacter>(AiController->GetPawn()))
		{
			if (const auto Blackboard = AiController->GetBlackboardComponent())
			{
				if (const auto CombatTarget = Cast<AActor>(Blackboard->GetValueAsObject(
					UUpBlueprintFunctionLibrary::GetBlackboardKeyFromTag(TAG_BlackboardKey_CombatTargetActor))))
				{
					const auto& Equipment = Character->GetEquipment();
					const auto EquipmentSlot = Equipment.GetPotentialActiveWeaponSlot();
					const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
					
					if ((CombatTarget->GetActorLocation() - Character->GetActorLocation()).Size() > 800.f)
					{
						if (EquipmentSlot != EUpEquipmentSlot::Weapon2 || !EquipmentSlotData.bActivated)
						{
							TriggerAbility(Character, EUpEquipmentSlot::Weapon2);
							return EBTNodeResult::InProgress;
						}
					} else if (EquipmentSlot != EUpEquipmentSlot::Weapon1 || !EquipmentSlotData.bActivated)
					{
						TriggerAbility(Character, EUpEquipmentSlot::Weapon1);
						return EBTNodeResult::InProgress;
					}
				}
				
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UUpBtTask_UpdateEquipment::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto AiPawn = Cast<AUpCharacter>(AiController->GetPawn()))
		{
			if (AiPawn->ShouldDebugGas())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s abort update equipment"), *AiPawn->GetName())
			}
			
			if (const auto AbilitySystemComponent = AiPawn->GetAbilitySystemComponent())
			{
				FGameplayTagContainer AbilityTags;
				AbilityTags.AddTag(TAG_Ability_ActivateEquipment);
				
				AbilitySystemComponent->CancelAbilities(&AbilityTags);
			}
		}
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UUpBtTask_UpdateEquipment::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (const auto Ability = AbilityEndedData.AbilityThatEnded; Ability && Ability->AbilityTags.HasTagExact(TAG_Ability_ActivateEquipment))
	{
		if (const auto AvatarActor = Cast<AUpCharacter>(Ability->GetAvatarActorFromActorInfo()))
		{
			if (AvatarActor->ShouldDebugGas())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s abilty ended %s"), *AvatarActor->GetName(), *Ability->GetName())
			}
			
			if (const auto AiController = Cast<AUpAiController>(AvatarActor->GetInstigatorController()))
			{
				if (const auto BlackboardComponent = AiController->GetBlackboardComponent())
				{
					if (const auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BlackboardComponent->GetBrainComponent()))
					{
						FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
					}
				}
			}
		}
	}
}

void UUpBtTask_UpdateEquipment::TriggerAbility(AUpCharacter* Character, const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto AbilitySystemComponent = Character->GetAbilitySystemComponent();
		AbilitySystemComponent && !AbilitySystemComponent->OnAbilityEnded.IsBoundToObject(this))
	{
		AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);
	}
	
	FGameplayEventData EventPayload;
	EventPayload.EventMagnitude = EquipmentSlot;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, TAG_Ability_ActivateEquipment, EventPayload);
}
