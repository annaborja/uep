// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_UpdateEquipment.h"

#include "AIController.h"
#include "Interfaces/UpCharacterEquippable.h"

EBTNodeResult::Type UUpBtTask_UpdateEquipment::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Equippable = Cast<IUpCharacterEquippable>(AiController->GetPawn()))
		{
			auto Equipment = Equippable->GetCharacterEquipment();

			switch (Command.CommandType)
			{
			case EUpUpdateEquipmentCommandType::Equip:
				if (const auto EquipmentSlotData = Equipment.GetEquipmentSlotData(Command.EquipmentSlot); EquipmentSlotData.IsValid())
				{
					Equippable->ActivateEquipment(Command.EquipmentSlot, EquipmentSlotData);
					
					return EBTNodeResult::Succeeded;
				}
				
				return EBTNodeResult::Failed;
			case EUpUpdateEquipmentCommandType::Unequip:
				Equippable->DeactivateEquipment(Command.EquipmentSlot);
				
				return EBTNodeResult::Succeeded;
			default:
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
