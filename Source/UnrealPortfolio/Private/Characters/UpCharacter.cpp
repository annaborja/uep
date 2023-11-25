// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpInventoryComponent.h"

AUpCharacter::AUpCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	InventoryComponent = CreateDefaultSubobject<UUpInventoryComponent>(TEXT("InventoryComponent"));
}
