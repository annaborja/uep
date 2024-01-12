// Copyright AB. All Rights Reserved.

#include "AI/EQS/UpEqsContext_PlayerCharacter.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UUpEqsContext_PlayerCharacter::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (const auto Character = PlayerController->GetCharacter())
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Character);
		}
	}
}
