// Copyright AB. All Rights Reserved.

#include "Items/UpLadder.h"

#include "Characters/UpPlayableCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Utils/Constants.h"

AUpLadder::AUpLadder()
{
	bCanInteract = true;

	if (const auto Mesh = GetStaticMeshComponent())
	{
		Mesh->SetCollisionProfileName(FName(TEXT("Custom")));
		Mesh->SetCollisionResponseToChannel(TRACE_CHANNEL_CLIMBABLE, ECR_Block);
	}

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(GetRootComponent());
	InteractionBox->SetRelativeLocation(FVector(8.0, 0.0, 120.0));
	InteractionBox->SetBoxExtent(FVector(8.0, 40.0, 180.0));
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

bool AUpLadder::Interact(AController* Controller)
{
	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		if (const auto Character = CustomPlayerController->GetPossessedCharacter())
		{
			if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
			{
				if (IsCharacterClimbing(Character))
				{
					CustomMovementComponent->StopClimb();
				} else
				{
					CustomMovementComponent->TryClimb(this);
				}
			}
		}
	}

	return true;
}

bool AUpLadder::CanInteract(const AController* Controller) const
{
	// if (const auto Character = PlayerController->GetPossessedCharacter()) {
	// 	const auto Angle = FMath::RadiansToDegrees(FMath::Acos(-Character->GetActorForwardVector().Dot(GetActorForwardVector())));
	//
	// 	if (bDebug)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Angle: %g"), Angle)
	// 	}
	// 	
	// 	if (Angle > 90.0) return false;
	// }
	
	return Super::CanInteract(Controller);
}

FText AUpLadder::GetInteractionPromptText(const AController* Controller) const
{
	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		const auto Character = CustomPlayerController->GetPossessedCharacter();
	
		return FText::FromString(FString::Printf(TEXT("%s %s"),
			Character && IsCharacterClimbing(Character) ? TEXT("Drop off") : TEXT("Climb"), *ItemData.Name.ToString()));
	}

	return FText::GetEmpty();
}

bool AUpLadder::IsCharacterClimbing(const AUpCharacter* Character) const
{
	if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
	{
		return CustomMovementComponent->GetClimbedActor() == this;
	}
	
	return false;
}
