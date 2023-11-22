// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpCharacterMovementComponent.h"
#include "UpPlayerMovementComponent.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpPlayerMovementComponent : public UUpCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UUpPlayerMovementComponent();

	virtual void BeginPlay() override;

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

private:
	UPROPERTY(EditAnywhere, Category="UP Params|Jump")
	float JumpCooldownTime = 0.1f;
	UPROPERTY(EditAnywhere, Category="UP Params|Jump")
	float JumpFallGravityScale = 4.f;
	
	FTimerHandle JumpCooldownTimerHandle;
	
	UFUNCTION()
	void AllowJump();
	UFUNCTION()
	void OnJumpApexReached() { GravityScale = JumpFallGravityScale; }
};
