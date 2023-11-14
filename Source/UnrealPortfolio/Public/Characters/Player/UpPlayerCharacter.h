// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "UpPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpCharacter
{
	GENERATED_BODY()

public:
	AUpPlayerCharacter();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> FollowCameraSpringArm;
};
