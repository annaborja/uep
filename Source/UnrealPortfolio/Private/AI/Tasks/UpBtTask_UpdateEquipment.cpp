// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_UpdateEquipment.h"

#include "AIController.h"
#include "Characters/UpCharacter.h"

EBTNodeResult::Type UUpBtTask_UpdateEquipment::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Character = Cast<AUpCharacter>(AiController->GetPawn()))
		{
			auto Equipment = Character->GetCharacterEquipment();

			switch (Command.CommandType)
			{
			case EUpUpdateEquipmentCommandType::Equip:
				if (const auto EquipmentSlotData = Equipment.GetEquipmentSlotData(Command.EquipmentSlot); EquipmentSlotData.IsValid())
				{
					Character->ActivateEquipment(Command.EquipmentSlot, EquipmentSlotData);
					
					return EBTNodeResult::Succeeded;
				}
				
				return EBTNodeResult::Failed;
			case EUpUpdateEquipmentCommandType::Unequip:
				Character->DeactivateEquipment(Command.EquipmentSlot);
				
				return EBTNodeResult::Succeeded;
			default:
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
