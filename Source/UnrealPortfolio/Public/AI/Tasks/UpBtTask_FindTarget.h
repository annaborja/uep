// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpBtTask_FindTarget.generated.h"

UENUM()
namespace EUpBttFindTargetSelectionStrategy
{
	enum Type : uint8
	{
		ClosestDistance,
		Random
	};
}

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_FindTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Debug")
	bool bDebugOverlapSphere = false;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FBlackboardKeySelector TargetSelector;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) };
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TSubclassOf<AActor> TargetClass;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FGameplayTag TargetTagId;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FGameplayTag TargetTag;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	bool bFindPlayerCharacter = false;
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	float OverlapSphereRadius = 5000.f;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TEnumAsByte<EUpBttFindTargetSelectionStrategy::Type> SelectionStrategy = EUpBttFindTargetSelectionStrategy::ClosestDistance;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	bool bFailIfNotFound = true;
};
