// Copyright AB. All Rights Reserved.

#include "Animation/UpCharacterAnimInstance.h"

#include "UpGameInstance.h"
#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Levels/UpLevelScriptActor.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	PrevYaw = Yaw;
	PrevRotationCurveValue = RotationCurveValue;
	
	if (const auto Character = Cast<AUpCharacter>(TryGetPawnOwner()))
	{
		const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(Character);
		const auto LevelScriptActor = GameInstance ? GameInstance->GetLevelScriptActor() : nullptr;
		
		HorizontalSpeed = Character->GetHorizontalSpeed();
		VerticalVelocity = Character->GetVelocity().Z;;
		bMovingHorizontally = HorizontalSpeed > 5.f;
		bMovingDownward = VerticalVelocity < -5.f;
		bMovingUpward = VerticalVelocity > 5.f;

		AimPitch = LevelScriptActor && LevelScriptActor->IsLookTargetActive() ? 0.f : Character->GetBaseAimRotation().Pitch;
		Yaw = Character->GetActorRotation().Yaw;
		MovementOffsetYaw = 0.f;

		if (const auto Mesh = Character->GetMesh())
		{
			if (const auto AnimInstance = Mesh->GetAnimInstance())
			{
				// Lateral movement animations look bad when combined with montage sequences,
				// so make sure the character is always animated as moving forward while a montage is playing.
				MovementOffsetYaw = Character->IsInStrafingMode() && !Character->IsShooting() && !AnimInstance->Montage_IsPlaying(nullptr) ? Character->GetMovementOffsetYaw() : 0.f;
			}
		}
		
		bUseBackwardsBlendSpace = !Character->IsInFirstPersonMode() && FMath::Abs(MovementOffsetYaw) > 120.f;
		
		CameraView = Character->GetCameraView();
		Posture = Character->GetPosture();
		bAiming = Character->IsAiming();
		bRelaxed = Character->IsRelaxed();

		if (const auto MovementComponent = Character->GetCustomMovementComponent())
		{
			bClimbingLadder = MovementComponent->IsClimbingLadder();
			bCrouching = MovementComponent->IsCrouching();
			bFalling = MovementComponent->IsFalling();
			bSprinting = MovementComponent->IsSprinting();

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

			// if (Character->ShouldDebugMovement())
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("%s"), "Debug movement")
			// }
		}
	}
}
