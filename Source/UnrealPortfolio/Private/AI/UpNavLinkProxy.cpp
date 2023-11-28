// Copyright AB. All Rights Reserved.

#include "AI/UpNavLinkProxy.h"

#include "Characters/UpNpcCharacter.h"
#include "Kismet/KismetMathLibrary.h"

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
		case EUpNavLinkProxyNavigationType::Mantle:
			{
				Npc->SetYaw(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DestinationPoint).Yaw);
			
				uint8 NumAttempts = 0;
			
				while (!Npc->Mantle())
				{
					// Avoid an infinite loop.
					if (++NumAttempts > 100) return;

					Npc->SetActorLocation(Npc->GetActorLocation() + Npc->GetActorForwardVector());
				}
			
				Npc->SetRootMotionTargetLocation(DestinationPoint);
				break;
			}
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid NavLinkProxy navigation type %d"), NavigationType)
		}
	}
}
