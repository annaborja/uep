// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_ExecuteDialogue.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_ExecuteDialogue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnDialogueEnded(AAIController* AiController);

	bool bDelegateAdded = false;
};
