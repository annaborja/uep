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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	float GroundSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	float VerticalSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	bool bStoppedWalking = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	bool bStoppedRunning = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	bool bIsCrouching = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	bool bIsFalling = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	bool bIsMovingHorizontally = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	TEnumAsByte<EUpCharacterArmingState::Type> ArmingState = EUpCharacterArmingState::Unarmed;
};
