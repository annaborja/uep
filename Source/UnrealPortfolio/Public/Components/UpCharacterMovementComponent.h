// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UpCharacterMovementComponent.generated.h"

UENUM()
namespace EUpCustomMovementMode
{
	enum Type : uint8
	{
		None
	};
}

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UUpCharacterMovementComponent();

	virtual void BeginPlay() override;
	virtual float GetMaxSpeed() const override;

	void ResetMaxWalkSpeed();
	void ResetRotationRate();

	void ToggleSprint(const bool bInWantsToSprint);

	FORCEINLINE float GetMaxSprintSpeed() const { return MaxSprintSpeed; }
	FORCEINLINE bool IsSprinting() const { return bWantsToSprint; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Params|Sprint")
	float MaxSprintSpeed = 700.f;

private:
	float BaseMaxWalkSpeed = 0.f;
	FRotator BaseRotationRate;

	bool bWantsToSprint = false;
};
