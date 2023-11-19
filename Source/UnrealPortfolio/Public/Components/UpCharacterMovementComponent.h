// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UpCharacterMovementComponent.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UUpCharacterMovementComponent();

	virtual void BeginPlay() override;

	void ResetMaxWalkSpeed();
	void ResetRotationRate();

private:
	float BaseMaxWalkSpeed = 0.f;
	FRotator BaseRotationRate;
};
