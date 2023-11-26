// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "GameplayEffect.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpInventoryComponent.h"

AUpCharacter::AUpCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InventoryComponent = CreateDefaultSubobject<UUpInventoryComponent>(TEXT("InventoryComponent"));
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(InitPrimaryAttributesEffectClass);
	check(InitVitalAttributesEffectClass);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());
}

void AUpCharacter::SetYaw(const float InYaw)
{
	SetActorRotation(FRotator(0.f, InYaw, 0.f));
}
