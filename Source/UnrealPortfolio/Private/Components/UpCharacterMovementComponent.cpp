// Copyright AB. All Rights Reserved.

#include "Components/UpCharacterMovementComponent.h"

UUpCharacterMovementComponent::UUpCharacterMovementComponent()
{
	MaxWalkSpeed = 600.f;
	bUseAccelerationForPaths = true;
}

void UUpCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseMaxWalkSpeed = MaxWalkSpeed;
	BaseRotationRate = RotationRate;
}

float UUpCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking && !IsCrouching() && bWantsToSprint) return MaxSprintSpeed;
	
	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();
	
	switch(CustomMovementMode)
	{
	case EUpCustomMovementMode::None:
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid custom movement mode %d"), CustomMovementMode)
		return MaxWalkSpeed;
	}
}

void UUpCharacterMovementComponent::ResetMaxWalkSpeed()
{
	MaxWalkSpeed = BaseMaxWalkSpeed;
}

void UUpCharacterMovementComponent::ResetRotationRate()
{
	RotationRate = BaseRotationRate;
}

void UUpCharacterMovementComponent::ToggleSprint(const bool bInWantsToSprint)
{
	bWantsToSprint = bInWantsToSprint;
}
