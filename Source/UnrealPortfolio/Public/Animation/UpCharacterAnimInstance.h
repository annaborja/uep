// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/UpCharacter.h"
#include "UpCharacterAnimInstance.generated.h"

class AUpCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	TEnumAsByte<EUpCameraView::Type> CameraView = EUpCameraView::ThirdPerson;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float HorizontalSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float VerticalVelocity = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float AimPitch = 0.f;

	// Difference between camera direction and movement direction, used for third-person strafing.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float MovementOffsetYaw = 0.f;
	// Used for rotating the root bone for turn-in-place logic.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float RootBoneOffsetYaw = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float TurningThresholdYaw = 90.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bMovingHorizontally = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bMovingDownward = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bMovingUpward = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bClimbingLadder = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bRelaxed = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bSprinting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bUseBackwardsBlendSpace = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;

private:
	float Yaw = 0.f;
	float PrevYaw = 0.f;
	
	float RotationCurveValue = 0.f;
	float PrevRotationCurveValue = 0.f;
};
