// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_SetMovementParams.generated.h"

UENUM()
namespace EUpBttSetMovementParamsCommandType
{
	enum Type : uint8
	{
		SetMaxWalkSpeed,
		
		SetRotationRate,
		ResetRotationRate,
		
		StartSprint,
		StopSprint,
		
		SetRelaxed
	};
}

USTRUCT()
struct FUpBttSetMovementParamsCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EUpBttSetMovementParamsCommandType::Type> CommandType = EUpBttSetMovementParamsCommandType::SetRelaxed;
	
	UPROPERTY(EditAnywhere)
	float FloatValue = 0.f;
	UPROPERTY(EditAnywhere)
	FVector VectorValue = FVector(0.0, 0.0, 0.0);
};

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_SetMovementParams : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params", meta=(TitleProperty="CommandType"))
	TArray<FUpBttSetMovementParamsCommand> Commands;
};
