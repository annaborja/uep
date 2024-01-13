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

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(GetRootComponent());
	InteractionBox->SetRelativeLocation(FVector(10.0, 0.0, 120.0));
	InteractionBox->SetBoxExtent(FVector(10.0, 40.0, 180.0));
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

void AUpLadder::Interact(AUpPlayerController* PlayerController)
{
	if (const auto Character = PlayerController->GetPossessedCharacter())
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

FText AUpLadder::GetInteractionPromptText(const AUpPlayerController* PlayerController) const
{
	const auto Character = PlayerController->GetPossessedCharacter();
	
	return FText::FromString(FString::Printf(TEXT("%s %s"),
		Character && IsCharacterClimbing(Character) ? TEXT("Drop off") : TEXT("Climb"), *ItemData.Name.ToString()));
}

bool AUpLadder::IsCharacterClimbing(const AUpCharacter* Character) const
{
	if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
	{
		return CustomMovementComponent->GetClimbedActor() == this;
	}
	
	return false;
}
