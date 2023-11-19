// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_GrantTagSpecs.generated.h"

struct FUpTagSpec;

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_GrantTagSpecs : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params", meta=(TitleProperty="Tag"))
	TArray<FUpTagSpec> TagSpecs;
};
