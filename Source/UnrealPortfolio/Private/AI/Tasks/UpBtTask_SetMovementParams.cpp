// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_SetMovementParams.h"

#include "AIController.h"
#include "Characters/UpPlayableNpc.h"
#include "Components/UpCharacterMovementComponent.h"

EBTNodeResult::Type UUpBtTask_SetMovementParams::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Npc = Cast<AUpPlayableNpc>(AiController->GetPawn()))
		{
			if (const auto MovementComponent = Npc->GetCustomMovementComponent())
			{
				for (const auto& Command : Commands)
				{
					switch (Command.CommandType)
					{
					case EUpBttSetMovementParamsCommandType::SetMaxWalkSpeed:
						MovementComponent->MaxWalkSpeed = Command.FloatValue;
						break;
					case EUpBttSetMovementParamsCommandType::ResetMaxWalkSpeed:
						MovementComponent->ResetMaxWalkSpeed();
						break;
					case EUpBttSetMovementParamsCommandType::SetRotationRate:
						MovementComponent->RotationRate = FRotator(
							Command.VectorValue.Y, Command.VectorValue.Z, Command.VectorValue.X);
						break;
					case EUpBttSetMovementParamsCommandType::ResetRotationRate:
						MovementComponent->ResetRotationRate();
						break;
					case EUpBttSetMovementParamsCommandType::StartSprint:
						Npc->ToggleSprint(true);
						break;
					case EUpBttSetMovementParamsCommandType::StopSprint:
						Npc->ToggleSprint(false);
						break;
					default:
						UE_LOG(LogTemp, Error, TEXT("Invalid SetMovementParams command type %d"), Command.CommandType)
						return EBTNodeResult::Failed;
					}
				}
				
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
