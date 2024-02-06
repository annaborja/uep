// Copyright AB. All Rights Reserved.

#include "AI/EQS/UpEqsContext_FactionCharacters.h"

#include "GenericTeamAgentInterface.h"
#include "AI/UpAiController.h"
#include "Characters/UpCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/KismetSystemLibrary.h"

void UUpEqsContext_FactionCharacters::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const auto Character = Cast<AUpCharacter>(QueryInstance.Owner))
	{
		if (const auto AiController = Cast<AUpAiController>(Character->GetController()))
		{
			TArray<AActor*> OverlapActors;
			UKismetSystemLibrary::SphereOverlapActors(Character, Character->GetActorLocation(), 2000.f,
				TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, AUpCharacter::StaticClass(),
				TArray<AActor*> { Character }, OverlapActors);

			for (const auto Actor : OverlapActors)
			{
				if (AiController->GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Friendly)
				{
					UEnvQueryItemType_Actor::SetContextHelper(ContextData, Actor);
				}
			}
		}
	}
}
