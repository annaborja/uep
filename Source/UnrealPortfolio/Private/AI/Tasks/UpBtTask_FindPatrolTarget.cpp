// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_FindPatrolTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpEnemyCharacter.h"
#include "Tags/ScriptTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

EBTNodeResult::Type UUpBtTask_FindPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (const auto AiController = OwnerComp.GetAIOwner())
		{
			if (const auto AiPawn = Cast<AUpEnemyCharacter>(AiController->GetPawn()))
			{
				if (const auto NewTarget = UUpBlueprintFunctionLibrary::FindPatrolTarget(AiPawn, OverlapSphereRadius, bDebugOverlapSphere))
				{
					BlackboardComponent->SetValueAsObject(UUpBlueprintFunctionLibrary::GetBlackboardKeyFromTag(TAG_BlackboardKey_PatrolTargetActor), NewTarget);
	
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
