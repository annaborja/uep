// Copyright AB. All Rights Reserved.

#include "Scripting/UpTargetActor.h"

#include "Components/SphereComponent.h"

AUpTargetActor::AUpTargetActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->SetCanEverAffectNavigation(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	for (const auto Channel : TArray<ECollisionChannel> { ECC_WorldDynamic, ECC_Pawn, ECC_Vehicle })
	{
		SphereComponent->SetCollisionResponseToChannel(Channel, ECR_Overlap);
	}
}
