// Copyright AB. All Rights Reserved.

#include "Animation/UpCharacterAnimInstance.h"

#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UUpCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const auto Character = Cast<AUpCharacter>(TryGetPawnOwner()))
	{
		if (const auto MovementComponent = Character->GetCustomMovementComponent())
		{
			const auto PrevGroundSpeed = GroundSpeed;
			const auto CurrentAcceleration = MovementComponent->GetCurrentAcceleration();
			const auto RunStopThreshold = MovementComponent->GetMaxSprintSpeed() * RunStopThresholdMultiplier;
			
			if (Character->ShouldDebugMovement())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s GroundSpeed prev = %g; Acceleration current = %s"),
					*Character->GetName(), PrevGroundSpeed, *CurrentAcceleration.ToString())
			}
			
			GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
			VerticalSpeed = MovementComponent->Velocity.Z;

			// TODO(P1): This logic works fine for NPCs but doesn't work well for the player character.
			bStoppedRunning = CurrentAcceleration.IsZero() && PrevGroundSpeed >= RunStopThreshold;
			bStoppedWalking = CurrentAcceleration.IsZero() && PrevGroundSpeed > 0.f && PrevGroundSpeed < RunStopThreshold;

			bIsCrouching = MovementComponent->IsCrouching();
			bIsFalling = MovementComponent->IsFalling();
		}
	}
}
