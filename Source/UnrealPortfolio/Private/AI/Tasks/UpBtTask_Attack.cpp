// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tags/GasTags.h"

EBTNodeResult::Type UUpBtTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (const auto AiController = OwnerComp.GetAIOwner())
		{
			if (const auto AiPawn = AiController->GetPawn())
			{
				if (const auto Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetSelector.SelectedKeyName)))
				{
					FGameplayEventData EventPayload;
					EventPayload.OptionalObject = Target;
		
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AiPawn, TAG_Ability_GunFire, EventPayload);
			
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
