// Copyright AB. All Rights Reserved.

#include "Components/UpCharacterMovementComponent.h"

#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerCharacter.h"

UUpCharacterMovementComponent::UUpCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;

	GravityScale = 2.f;
	JumpZVelocity = 640.f;
	MaxWalkSpeed = 400.f;
	MaxWalkSpeedCrouched = 135.f;
	bUseAccelerationForPaths = true;
}

void UUpCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<AUpCharacter>(GetOwner());
	Npc = Cast<AUpNpcCharacter>(GetOwner());

	BaseGravityScale = GravityScale;
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
		return 0.f;
	}
}
