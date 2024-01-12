// Copyright AB. All Rights Reserved.

#include "AI/Tasks/UpBtTask_SetBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UUpBtTask_SetBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (bClear)
		{
			BlackboardComponent->ClearValue(TargetSelector.SelectedKeyName);
			
			return EBTNodeResult::Succeeded;
		}
		
		switch (ValueType)
		{
		case EUpBlackboardValueType::Boolean:
			BlackboardComponent->SetValueAsBool(TargetSelector.SelectedKeyName, bBooleanValue);
			
			return EBTNodeResult::Succeeded;
		case EUpBlackboardValueType::Vector:
			BlackboardComponent->SetValueAsVector(TargetSelector.SelectedKeyName, VectorValue);
			
			return EBTNodeResult::Succeeded;
		default:
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}
