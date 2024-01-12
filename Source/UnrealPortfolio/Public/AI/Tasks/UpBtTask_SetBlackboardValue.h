// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_SetBlackboardValue.generated.h"

UENUM(BlueprintType)
namespace EUpBlackboardValueType
{
	enum Type : uint8
	{
		Boolean,
		Vector
	};
}

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_SetBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FBlackboardKeySelector TargetSelector;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	bool bClear = false;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TEnumAsByte<EUpBlackboardValueType::Type> ValueType = EUpBlackboardValueType::Boolean;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	bool bBooleanValue = false;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FVector VectorValue;
};
