// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_SetMovementParams.h"

#include "AIController.h"
#include "Characters/UpPlayableNpc.h"
#include "Components/UpCharacterMovementComponent.h"

EBTNodeResult::Type UUpBtTask_SetMovementParams::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto AiController = OwnerComp.GetAIOwner())
	{
		if (const auto Character = Cast<AUpCharacter>(AiController->GetPawn()))
		{
			if (const auto MovementComponent = Character->GetCustomMovementComponent())
			{
				for (const auto& Command : Commands)
				{
					switch (Command.CommandType)
					{
					case EUpBttSetMovementParamsCommandType::SetMaxWalkSpeed:
						MovementComponent->SetOverrideMaxWalkSpeed(Command.FloatValue);
						break;
					case EUpBttSetMovementParamsCommandType::SetRotationRate:
						MovementComponent->RotationRate = FRotator(
							Command.VectorValue.Y, Command.VectorValue.Z, Command.VectorValue.X);
						break;
					case EUpBttSetMovementParamsCommandType::ResetRotationRate:
						MovementComponent->ResetRotationRate();
						break;
					case EUpBttSetMovementParamsCommandType::StartSprint:
						Character->ToggleSprint(true);
						break;
					case EUpBttSetMovementParamsCommandType::StopSprint:
						Character->ToggleSprint(false);
						break;
					case EUpBttSetMovementParamsCommandType::SetRelaxed:
						Character->SetRelaxed(Command.FloatValue > 0.f);
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
