// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UpCharacterMovementComponent.generated.h"

class AUpCharacter;
class AUpNpcCharacter;
class AUpPlayerCharacter;

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

	void ToggleSprint(const bool bInWantsToSprint) { bWantsToSprint = bInWantsToSprint; }

	void ResetMaxWalkSpeed() { MaxWalkSpeed = BaseMaxWalkSpeed; }
	void ResetRotationRate() { RotationRate = BaseRotationRate; }
	
	FORCEINLINE float GetMaxSprintSpeed() const { return MaxSprintSpeed; }
	FORCEINLINE bool IsSprinting() const { return bWantsToSprint; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Params|Sprint")
	float MaxSprintSpeed = 700.f;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerCharacter> Player;
	
	float BaseGravityScale = 1.f;

private:
	UPROPERTY(Transient)
	TObjectPtr<AUpCharacter> Character;
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;
	
	float BaseMaxWalkSpeed = 0.f;
	FRotator BaseRotationRate;

	bool bWantsToSprint = false;
};
