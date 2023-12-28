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
				if (Character->ActivateEquipment(Command.EquipmentSlot)) return EBTNodeResult::Succeeded;
				
				return EBTNodeResult::Failed;
			case EUpUpdateEquipmentCommandType::Unequip:
				if (Character->DeactivateEquipment(Command.EquipmentSlot)) return EBTNodeResult::Succeeded;
				
				return EBTNodeResult::Failed;
			default:
				return EBTNodeResult::Failed;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
