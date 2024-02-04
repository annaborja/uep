// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_FindPatrolTarget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_FindPatrolTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Debug")
	bool bDebugOverlapSphere = false;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	float OverlapSphereRadius = 5000.f;
};
