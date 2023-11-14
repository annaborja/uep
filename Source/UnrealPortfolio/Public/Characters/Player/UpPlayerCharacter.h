// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "UpPlayerCharacter.generated.h"

class AUpPlayerController;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpCharacter
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> FollowCameraSpringArm;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
};
