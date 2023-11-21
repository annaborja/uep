// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCameraManager.h"

#include "Characters/Player/UpPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUpPlayerCameraManager::AUpPlayerCameraManager(): APlayerCameraManager()
{
	ViewPitchMax = 65.f;
	ViewPitchMin = -70.f;
}

void AUpPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Controller = GetOwningPlayerController())
	{
		Player = CastChecked<AUpPlayerCharacter>(Controller->GetCharacter());
	}
}

void AUpPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, const float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	AdjustCameraForCrouch(OutVT, DeltaTime);
}

void AUpPlayerCameraManager::AdjustCameraForCrouch(FTViewTarget& OutVT, const float DeltaTime)
{
	if (!Player) return;

	if (const auto MovementComponent = Player->GetCharacterMovement(); MovementComponent && MovementComponent->IsMovingOnGround())
	{
		// Read the capsule half-height from the default object
		// since the capsule half-height of the character instance will change based on its crouch state.
		const auto DefaultCapsuleHalfHeight = Player->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		const FVector TargetCrouchOffset(0, 0, MovementComponent->GetCrouchedHalfHeight() - DefaultCapsuleHalfHeight);
		
		auto Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset,
			FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (MovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			// Negate the immediate downward movement of the camera that occurs automatically when crouching
			// (this logic might seem a bit unintuitive, but it makes sense if you picture
			// how the offset magnitude changes as CrouchBlendTime goes from 0.0 to 1.0 and back).
			Offset -= TargetCrouchOffset;
		} else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
