// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "Components/UpCharacterMovementComponent.h"

AUpCharacter::AUpCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());
}
