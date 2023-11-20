// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_FindTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

EBTNodeResult::Type UUpBtTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* NewTarget = nullptr;

	// If no target class is set, set the blackboard value to null.
	if (TargetClass)
	{
		if (const auto AiController = OwnerComp.GetAIOwner())
		{
			if (const auto AiPawn = AiController->GetPawn())
			{
				const auto SpherePosition = AiPawn->GetActorLocation();

				TArray<AActor*> OverlapActors;
				UKismetSystemLibrary::SphereOverlapActors(AiPawn, SpherePosition, OverlapSphereRadius,
					TargetObjectTypes, TargetClass, TArray<AActor*> { AiPawn }, OverlapActors);

				if (bDebugOverlapSphere)
				{
					UKismetSystemLibrary::DrawDebugSphere(AiPawn, SpherePosition, OverlapSphereRadius, 12, FColor::Orange, 5.f);
				}

				TArray<AActor*> CandidateActors;

				for (const auto Actor : OverlapActors)
				{
					if (TargetTagId.IsValid() && UUpBlueprintFunctionLibrary::HasTagId(Actor, TargetTagId))
					{
						CandidateActors.Add(Actor);
						break;
					}

					if ((TargetTag.IsValid() && UUpBlueprintFunctionLibrary::HasTag(Actor, TargetTag)) ||
						(!TargetTagId.IsValid() && !TargetTag.IsValid()))
					{
						CandidateActors.Add(Actor);
					}
				}

				if (CandidateActors.Num() > 0)
				{
					int32 NewTargetIndex = 0;

					if (CandidateActors.Num() > 1)
					{
						switch (SelectionStrategy)
						{
						case EUpBttFindTargetSelectionStrategy::Random:
							NewTargetIndex = FMath::RandRange(0, CandidateActors.Num() - 1);
							break;
						case EUpBttFindTargetSelectionStrategy::ClosestDistance:
						default:
							CandidateActors.Sort([AiPawn](const AActor& ActorA, const AActor& ActorB)
							{
								return AiPawn->GetDistanceTo(&ActorA) < AiPawn->GetDistanceTo(&ActorB);
							});
						}
					}

					if (CandidateActors.IsValidIndex(NewTargetIndex))
					{
						NewTarget = CandidateActors[NewTargetIndex];
					}
				}
			}
		}
	}

	if (NewTarget || !bFailIfNotFound)
	{
		if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(TargetSelector.SelectedKeyName, NewTarget);
		
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
