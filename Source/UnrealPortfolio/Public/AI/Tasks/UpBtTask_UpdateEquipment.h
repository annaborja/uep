// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Utils/Enums.h"
#include "UpBtTask_UpdateEquipment.generated.h"

UENUM()
namespace EUpUpdateEquipmentCommandType
{
	enum Type : uint8
	{
		Equip,
		Unequip
	};
}

USTRUCT()
struct FUpUpdateEquipmentCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EUpUpdateEquipmentCommandType::Type> CommandType = EUpUpdateEquipmentCommandType::Equip;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EUpEquipmentSlot::Type> EquipmentSlot = EUpEquipmentSlot::Weapon1;
};

UCLASS()
class UNREALPORTFOLIO_API UUpBtTask_UpdateEquipment : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditInstanceOnly, Category="UP Params", meta=(TitleProperty="CommandType"))
	FUpUpdateEquipmentCommand Command;
};
