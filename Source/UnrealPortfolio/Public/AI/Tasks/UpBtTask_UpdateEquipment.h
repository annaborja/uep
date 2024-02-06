// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Utils/Enums.h"
#include "UpBtTask_UpdateEquipment.generated.h"

class AUpCharacter;
struct FAbilityEndedData;

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_UpdateEquipment : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	void TriggerAbility(AUpCharacter* Character, const EUpEquipmentSlot::Type EquipmentSlot);
};
