// Copyright AB. All Rights Reserved.

#include "Animation/UpCharacterAnimInstance.h"

#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UUpCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	PrevYaw = Yaw;
	PrevRotationCurveValue = RotationCurveValue;
	
	if (const auto Character = Cast<AUpCharacter>(TryGetPawnOwner()))
	{
		Yaw = Character->GetActorRotation().Yaw;
		AimPitch = Character->GetBaseAimRotation().Pitch;
		
		CameraView = Character->GetCameraView();
		Posture = Character->GetPosture();
		bRelaxed = Character->IsRelaxed();
		
		if (Character->IsInStrafingMode())
		{
			MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
				UKismetMathLibrary::MakeRotFromX(Character->GetVelocity()), Character->GetBaseAimRotation()).Yaw;
		} else
		{
			MovementOffsetYaw = 0.f;
		}

		if (const auto MovementComponent = Character->GetCustomMovementComponent())
		{
			HorizontalSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
			VerticalVelocity = MovementComponent->Velocity.Z;
			
			bMovingHorizontally = HorizontalSpeed > 5.f;
			bMovingDownward = VerticalVelocity < -5.f;
			bMovingUpward = VerticalVelocity > 5.f;
			
			bClimbingLadder = MovementComponent->IsClimbingLadder();
			bCrouching = MovementComponent->IsCrouching();
			bFalling = MovementComponent->IsFalling();
			bSprinting = HorizontalSpeed > MovementComponent->MaxWalkSpeed + 5.f;

			//// TODO(P2): Remove
			MaxWalkSpeed = MovementComponent->MaxWalkSpeed;
			GroundSpeed = HorizontalSpeed;
			VerticalSpeed = VerticalVelocity;
			bIsCrouching = bCrouching;
			bIsFalling = bFalling;
			bIsMovingHorizontally = bMovingHorizontally;
			////

			// NOTE: Attempting to implement turn-in-place logic resulted in animation wonkiness,
			// so the following logic is commented out for now.
			//
			// if (!bMovingHorizontally && !bFalling && CameraView == EUpCameraView::ThirdPerson_OverTheShoulder)
			// {
			// 	RootBoneOffsetYaw = UKismetMathLibrary::NormalizeAxis(RootBoneOffsetYaw - (Yaw - PrevYaw));
			//
			// 	// Note that getting curve values won't work if you're using different skeletons for your character and the animation
			// 	// (see https://forums.unrealengine.com/t/getting-curve-value-from-animation-sequence-return-zero/482511/6).
			// 	if (GetCurveValue(TEXT("Turning")) > 0.0)
			// 	{
			// 		RotationCurveValue = GetCurveValue(TEXT("Rotation"));
			//
			// 		const auto DeltaRotation = RotationCurveValue - PrevRotationCurveValue;
			//
			// 		if (RootBoneOffsetYaw > 0.f) // Turning left.
			// 		{
			// 			RootBoneOffsetYaw -= DeltaRotation;
			// 		} else
			// 		{
			// 			RootBoneOffsetYaw += DeltaRotation;
			// 		}
			//
			// 		if (const auto RootBoneOffsetYaw_Abs = FMath::Abs(RootBoneOffsetYaw); RootBoneOffsetYaw_Abs > TurningThresholdYaw)
			// 		{
			// 			const auto ExcessYaw = RootBoneOffsetYaw_Abs - TurningThresholdYaw;
			//
			// 			if (RootBoneOffsetYaw > 0.f)
			// 			{
			// 				RootBoneOffsetYaw -= ExcessYaw;
			// 			} else
			// 			{
			// 				RootBoneOffsetYaw += ExcessYaw;
			// 			}
			// 		}
			// 	} else
			// 	{
			// 		RotationCurveValue = 0.f;
			// 	}
			// } else
			// {
			// 	RootBoneOffsetYaw = 0.f;
			// 	RotationCurveValue = 0.f;
			// }

			if (Character->ShouldDebugMovement())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), "Debug movement")
			}
		}
	}
}
