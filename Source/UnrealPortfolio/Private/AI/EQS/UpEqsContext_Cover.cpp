// Copyright AB. All Rights Reserved.

#include "AI/EQS/UpEqsContext_Cover.h"

#include "Characters/UpCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/KismetSystemLibrary.h"

void UUpEqsContext_Cover::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const auto Character = Cast<AUpCharacter>(QueryInstance.Owner))
	{
		TArray<AActor*> OverlapActors;
		UKismetSystemLibrary::SphereOverlapActors(Character, Character->GetActorLocation(), 5000.f,
			TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) }, AActor::StaticClass(),
			TArray<AActor*> { Character }, OverlapActors);

		for (const auto Actor : OverlapActors)
		{
			if (Actor->GetComponentsCollisionResponseToChannel(ECC_WorldStatic) == ECR_Block)
			{
				UE_LOG(LogTemp, Warning, TEXT("cover %s"), *Actor->GetName())
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, Actor);
			}
		}
	}
}
