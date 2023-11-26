// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerMovementComponent.h"

#include "Characters/Player/UpPlayerCharacter.h"

UUpPlayerMovementComponent::UUpPlayerMovementComponent()
{
	bOrientRotationToMovement = true;
}

void UUpPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = CastChecked<AUpPlayerCharacter>(GetOwner());
	Player->OnReachedJumpApex.AddDynamic(this, &ThisClass::OnJumpApexReached);
}

void UUpPlayerMovementComponent::UpdateCharacterStateBeforeMovement(const float DeltaSeconds)
{
	// If the player is trying to jump...
	if (bCustomPressedJump)
	{
		// Try mantling.
		if (TryMantle())
		{
			if (Player) Player->StopJumping();
		} else
		{
			// If mantling didn't work, revert to UE's default jump behavior.
			bCustomPressedJump = false;

			if (Player)
			{
				Player->bPressedJump = true;
				Player->CheckJumpInput(DeltaSeconds);
			}
		}
	} else if (MovementMode == MOVE_Falling)
	{
		TryMantle();
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UUpPlayerMovementComponent::OnMovementModeChanged(const EMovementMode PreviousMovementMode, const uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Falling)
	{
		GravityScale = BaseGravityScale;
	}

	// We use the flying movement mode for special jump mechanics like mantling.
	if (PreviousMovementMode == MOVE_Falling || PreviousMovementMode == MOVE_Flying)
	{
		if (Player && !Player->IsAllowedToJump())
		{
			Player->GetWorldTimerManager().SetTimer(JumpCooldownTimerHandle, this, &ThisClass::AllowJump, JumpCooldownTime);
		}
	}
}

void UUpPlayerMovementComponent::AllowJump()
{
	if (!Player) return;

	Player->AllowJump();
}
