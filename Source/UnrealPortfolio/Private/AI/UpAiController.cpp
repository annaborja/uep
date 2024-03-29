// Copyright AB. All Rights Reserved.

#include "AI/UpAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AUpAiController::AUpAiController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	PerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	PerceptionComponent->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::HandleTargetPerceptionUpdated);
}

ETeamAttitude::Type AUpAiController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const auto Pawn = Cast<APawn>(&Other))
	{
		const auto TeamId = GetGenericTeamId().GetId();

		if (Cast<AUpPlayerController>(Pawn->GetController()))
		{
			if (TeamId == 1) return ETeamAttitude::Friendly;
			if (TeamId == 0) return ETeamAttitude::Neutral;
		} else if (const auto TeamAgent = Cast<IGenericTeamAgentInterface>(Pawn->GetController()))
		{
			const auto OtherTeamId = TeamAgent->GetGenericTeamId().GetId();

			if (OtherTeamId == TeamId) return ETeamAttitude::Friendly;
			if (OtherTeamId == 0) return ETeamAttitude::Neutral;
		}
	}
	
	return ETeamAttitude::Hostile;
}

void AUpAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetGenericTeamId(FGenericTeamId(Cast<AUpPlayableCharacter>(InPawn) ? 1 : 2));

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

	if (IsValid(this)) Destroy();
}

void AUpAiController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// UE_LOG(LogTemp, Warning, TEXT("perception uddated %s"), *Actor->GetName())
}

