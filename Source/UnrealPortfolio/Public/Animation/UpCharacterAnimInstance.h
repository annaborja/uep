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
	UPROPERTY(EditAnywhere, Category="UP Params")
	float RunStopThresholdMultiplier = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float GroundSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float VerticalSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	float MaxSprintSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bStoppedWalking = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bStoppedRunning = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bIsCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bIsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bIsMovingHorizontally = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	bool bRelaxed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UP Runtime")
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;
};
