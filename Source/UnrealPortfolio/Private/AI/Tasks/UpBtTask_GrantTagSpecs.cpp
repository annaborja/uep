// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_GrantTagSpecs.h"

#include "AIController.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "Utils/Structs.h"

EBTNodeResult::Type UUpBtTask_GrantTagSpecs::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto TagSpecGrantable = Cast<IUpTagSpecGrantable>(AiController->GetPawn()))
		{
			for (const auto& TagSpec : TagSpecs)
			{
				TagSpecGrantable->GrantTagSpec(TagSpec);
			}
			
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
