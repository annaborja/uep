// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UpCharacterAnimInstance.generated.h"

class AUpCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UP Runtime")
	float GroundSpeed = 0.f;
};
