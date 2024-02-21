// Copyright AB. All Rights Reserved.

#include "AI/UpNavLinkProxy.h"

#include "Characters/UpPlayableNpc.h"
#include "Components/CapsuleComponent.h"
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

				FHitResult OutHit;
				UKismetSystemLibrary::CapsuleTraceSingle(this, Npc->GetActorLocation(), DestinationPoint, 20.f, 5.f,
					UEngineTypes::ConvertToTraceType(ECC_Pawn), false, TArray<AActor*> { Npc },
					bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHit, true);

				if (OutHit.bBlockingHit && Cast<AUpCharacter>(OutHit.GetActor()))
				{
					// UE_LOG(LogTemp, Warning, TEXT("%s blocking hit: %s"), *Npc->GetName(), *OutHit.GetActor()->GetName())
					return;
				}

				// const auto LaunchVector(DestinationPoint - Npc->GetActorLocation());
				// Npc->LaunchCharacter(FVector(LaunchVector.X, LaunchVector.Y, 0.f), true, false);

				if (Npc->Mantle())
				{
					Npc->SetRootMotionTargetLocation(DestinationPoint);

					// if (const auto Capsule = Npc->GetCapsuleComponent())
					// {
					// 	TArray<AActor*> OutActors;
					// 	UKismetSystemLibrary::CapsuleOverlapActors(this, DestinationPoint, Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(),
					// 		TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, nullptr,
					// 		TArray<AActor*> { Npc }, OutActors);
					// 	UKismetSystemLibrary::DrawDebugCapsule(this, DestinationPoint, Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Npc->GetActorRotation());
					//
					// 	for (const auto Actor : OutActors)
					// 	{
					// 		if (const auto Character = Cast<AUpCharacter>(Actor))
					// 		{
					// 			UE_LOG(LogTemp, Warning, TEXT("Launch: %s"), *Character->GetName())
					// 			// Character->SetActorLocation(Character->GetActorLocation() + Character->GetActorForwardVector() * 50.f + Character->GetActorUpVector() * 50.f);
					// 			Character->LaunchCharacter(Character->GetActorForwardVector() * 200.f + Character->GetActorUpVector() * 100.f, true, true);
					// 		}
					// 	}
					// }
				}
				
				break;
			}
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid NavLinkProxy navigation type %d"), NavigationType)
		}
	}
}
