// Copyright AB. All Rights Reserved.

#include "AI/UpNavLinkProxy.h"

#include "Characters/UpPlayableNpc.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Items/UpDoor.h"
#include "Kismet/GameplayStatics.h"
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

	switch (NavigationType)
	{
	case EUpNavLinkProxyNavigationType::Door:
		{
			TArray<AActor*> OutActors;
			UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), OverlapSphereRadius,
				TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) }, AUpDoor::StaticClass(),
				TArray<AActor*> { this }, OutActors);

			if (OutActors.Num() != 1)
			{
				UE_LOG(LogTemp, Error, TEXT("Unexpected number of doors found: %d"), OutActors.Num())
			}
			
			for (const auto Actor : OutActors)
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *Actor->GetName())
				}

				AssociatedActor = Actor;
			}
			
			break;
		}
	default:
		break;
	}
}

void AUpNavLinkProxy::HandleSmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	if (const auto Npc = Cast<AUpPlayableNpc>(MovingActor))
	{
		switch (NavigationType)
		{
		case EUpNavLinkProxyNavigationType::Door:
			{
				if (const auto Door = Cast<AUpDoor>(AssociatedActor))
				{
					const auto PushNpcThroughDoor = [Npc, DestinationPoint](const bool bOpen)
					{
						if (!bOpen) return;
						
						if (const auto CustomMovementComponent = Npc->GetCustomMovementComponent())
						{
							CustomMovementComponent->StopMovementImmediately();
						}
						
						Npc->SetYaw(UKismetMathLibrary::FindLookAtRotation(Npc->GetActorLocation(), DestinationPoint).Yaw);
						Npc->LaunchCharacter(DestinationPoint - Npc->GetActorLocation(), true, true);
					};
					
					if (Door->IsOpen())
					{
						PushNpcThroughDoor(true);
					} else
					{
						Door->OpenStateDelegate.Clear();
						Door->OpenStateDelegate.AddLambda(PushNpcThroughDoor);
						Door->Open();
					}
				}
				
				break;
			}
		case EUpNavLinkProxyNavigationType::Jump:
			Npc->JumpToLocation(DestinationPoint, MovementDuration);
			break;
		case EUpNavLinkProxyNavigationType::Mantle:
			{
				if (const auto CustomMovementComponent = Npc->GetCustomMovementComponent())
				{
					CustomMovementComponent->StopMovementImmediately();
				}
				
				Npc->SetYaw(UKismetMathLibrary::FindLookAtRotation(Npc->GetActorLocation(), DestinationPoint).Yaw);

				const auto LaunchVector(DestinationPoint - Npc->GetActorLocation());
				Npc->LaunchCharacter(FVector(LaunchVector.X, LaunchVector.Y, 0.f), true, false);

				if (Npc->Mantle()) Npc->SetRootMotionTargetLocation(DestinationPoint);
				
				break;
			}
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid NavLinkProxy navigation type %d"), NavigationType)
		}
	}
}
