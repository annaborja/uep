// Copyright AB. All Rights Reserved.

#include "Characters/UpNonPlayableNpc.h"

#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/UpCharacterMovementComponent.h"

AUpNonPlayableNpc::AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AIControllerClass = AUpAiController::StaticClass();
}

void AUpNonPlayableNpc::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	AiController = Cast<AUpAiController>(NewController);

	if (AiController && BehaviorTree)
	{
		if (const auto BlackboardComponent = AiController->GetBlackboardComponent())
		{
			if (const auto BlackboardAsset = BehaviorTree->BlackboardAsset)
			{
				BlackboardComponent->InitializeBlackboard(*BlackboardAsset);
				AiController->RunBehaviorTree(BehaviorTree);
			}
		}
	}
}
