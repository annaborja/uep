// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_ExecuteDialogue.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/UpDialogueComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UUpBtTask_ExecuteDialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	DialogueEndedDelegate.BindUFunction(this, FName("OnDialogueEnded"));
	
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Npc = Cast<AUpNpcCharacter>(AiController->GetPawn()))
		{
			if (const auto DialogueComponent = Npc->GetDialogueComponent(); DialogueComponent && DialogueComponent->HasAvailableDialogue())
			{
				if (const auto PlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
				{
					if (!DialogueComponent->DialogueEndedDelegate.Contains(DialogueEndedDelegate))
					{
						DialogueComponent->DialogueEndedDelegate.Add(DialogueEndedDelegate);
					}
					
					DialogueComponent->StartDialogue(PlayerController);
					
					return EBTNodeResult::InProgress;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}

void UUpBtTask_ExecuteDialogue::OnDialogueEnded(AAIController* AiController)
{
	if (const auto BlackboardComponent = AiController->GetBlackboardComponent())
	{
		if (const auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BlackboardComponent->GetBrainComponent()))
		{
			FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
		}
	}
}
