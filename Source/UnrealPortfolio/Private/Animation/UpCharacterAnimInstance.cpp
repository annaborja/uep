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
		Posture = Character->GetPosture();
		bRelaxed = Character->IsRelaxed();
		
		if (const auto MovementComponent = Character->GetCustomMovementComponent())
		{
			GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
			VerticalSpeed = MovementComponent->Velocity.Z;

			MaxWalkSpeed = MovementComponent->MaxWalkSpeed;
			MaxSprintSpeed = MovementComponent->GetMaxSprintSpeed();

			bClimbingLadder = MovementComponent->IsClimbingLadder();
			bIsCrouching = MovementComponent->IsCrouching();
			bIsFalling = MovementComponent->IsFalling();
			bIsMovingHorizontally = GroundSpeed > 5.f;
		}
	}
}
