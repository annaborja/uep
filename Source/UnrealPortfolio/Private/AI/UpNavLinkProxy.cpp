// Copyright AB. All Rights Reserved.

#include "AI/UpNavLinkProxy.h"

#include "Characters/UpNpcCharacter.h"

AUpNavLinkProxy::AUpNavLinkProxy()
{
	PointLinks = TArray<FNavigationLink>();
	bSmartLinkIsRelevant = true;
}

void AUpNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &ThisClass::HandleSmartLinkReached);
}

void AUpNavLinkProxy::HandleSmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	if (const auto Npc = Cast<AUpNpcCharacter>(MovingActor))
	{
		switch (NavigationType)
		{
		case EUpNavLinkProxyNavigationType::Jump:
			Npc->JumpToLocation(DestinationPoint, MovementDuration);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid NavLinkProxy navigation type %d"), NavigationType)
		}
	}
}
