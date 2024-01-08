// Copyright AB. All Rights Reserved.

#include "AI/UpAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "Navigation/CrowdFollowingComponent.h"

AUpAiController::AUpAiController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void AUpAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto Character = Cast<AUpCharacter>(InPawn))
	{
		if (const auto BehaviorTree = Character->GetBehaviorTree())
		{
			if (const auto BlackboardAsset = BehaviorTree->BlackboardAsset)
			{
				if (const auto BlackboardComponent = GetBlackboardComponent())
				{
					BlackboardComponent->InitializeBlackboard(*BlackboardAsset);
					RunBehaviorTree(BehaviorTree);
				}
			}
		}
	}
}

void AUpAiController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BrainComponent) BrainComponent->StopLogic(TEXT("Unpossessed"));

	if (IsValid(this)) Destroy();
}

