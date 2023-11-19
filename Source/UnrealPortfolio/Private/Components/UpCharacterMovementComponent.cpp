// Copyright AB. All Rights Reserved.

#include "Components/UpCharacterMovementComponent.h"

UUpCharacterMovementComponent::UUpCharacterMovementComponent()
{
	MaxWalkSpeed = 600.f;
}

void UUpCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseMaxWalkSpeed = MaxWalkSpeed;
	BaseRotationRate = RotationRate;
}

void UUpCharacterMovementComponent::ResetMaxWalkSpeed()
{
	MaxWalkSpeed = BaseMaxWalkSpeed;
}

void UUpCharacterMovementComponent::ResetRotationRate()
{
	RotationRate = BaseRotationRate;
}
