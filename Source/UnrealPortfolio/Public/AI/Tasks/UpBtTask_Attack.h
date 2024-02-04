// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GAS/Abilities/UpDeathAbility.h"
#include "UpBtTask_Attack.generated.h"

struct FAbilityEndedData;

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	TMulticastDelegate<void(const FAbilityEndedData&)>::FDelegate AttackEndedDelegate;
	
	UFUNCTION()
	void OnAttackEnded(const FAbilityEndedData& AbilityEndedData);
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FBlackboardKeySelector TargetSelector;
};
