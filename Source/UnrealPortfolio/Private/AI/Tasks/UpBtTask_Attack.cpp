// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "Tags/GasTags.h"

EBTNodeResult::Type UUpBtTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (const auto AiController = OwnerComp.GetAIOwner())
		{
			if (const auto AiPawn = Cast<AUpCharacter>(AiController->GetPawn()))
			{
				if (const auto Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetSelector.SelectedKeyName)))
				{
					FGameplayEventData EventPayload;
					EventPayload.OptionalObject = Target;
		
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AiPawn, TAG_Ability_GunFire, EventPayload);

					if (const auto AbilitySystemComponent = AiPawn->GetAbilitySystemComponent();
						AbilitySystemComponent && !AbilitySystemComponent->OnAbilityEnded.IsBoundToObject(this))
					{
						AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnAttackEnded);
					}
			
					return EBTNodeResult::InProgress;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}

void UUpBtTask_Attack::OnAttackEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (const auto Ability = AbilityEndedData.AbilityThatEnded; Ability && Ability->AbilityTags.HasTagExact(TAG_Ability_GunFire))
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
