// Copyright AB. All Rights Reserved.

#include "AI/EQS/UpEqsContext_CombatTarget.h"

#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Tags/ScriptTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpEqsContext_CombatTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const auto Character = Cast<AUpCharacter>(QueryInstance.Owner))
	{
		if (const auto AiController = Cast<AUpAiController>(Character->GetController()))
		{
			if (const auto BlackboardComponent = AiController->GetBlackboardComponent())
			{
				if (const auto Actor = Cast<AActor>(BlackboardComponent->GetValueAsObject(
					UUpBlueprintFunctionLibrary::GetBlackboardKeyFromTag(TAG_BlackboardKey_CombatTargetActor))))
				{
					UEnvQueryItemType_Actor::SetContextHelper(ContextData, Actor);
				}
			}
		}
	}
}
