// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/Structs.h"
#include "UpCharacterMovementComponent.generated.h"

class AUpCharacter;
class AUpPlayableCharacter;

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
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	void InitForPlayer();
	void TearDownForPlayer();

	void ResetMaxWalkSpeed() { MaxWalkSpeed = BaseMaxWalkSpeed; }
	void ResetRotationRate() { RotationRate = BaseRotationRate; }
	void ToggleCustomPressedJump(const bool bInCustomPressedJump) { bCustomPressedJump = bInCustomPressedJump; }
	void ToggleSprint(const bool bInWantsToSprint) { bWantsToSprint = bInWantsToSprint; }
	bool TryMantle();

	FORCEINLINE float GetMaxSprintSpeed() const { return MaxSprintSpeed; }
	FORCEINLINE bool IsSprinting() const { return bWantsToSprint; }
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMantle = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;

	UPROPERTY(EditAnywhere, Category="UP Params|Jump")
	float JumpCooldownTime = 0.1f;
	UPROPERTY(EditAnywhere, Category="UP Params|Jump")
	float JumpFallGravityScale = 4.f;

	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	uint8 NumMantleFrontTraceDivisions = 8;
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleMinDistanceOffset = 30.f;
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleMaxDistance = 200.f;
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleReachHeight = 50.f;
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MinMantleDepth = 30.f;
	/** The minimum valid angle between the world up vector and the surface impact normal. */
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleMinWallSteepnessAngle = 75.f;
	/** The maximum valid angle between the character's velocity forward vector and the inverse surface impact normal. */
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleMaxSurfaceAngle = 40.f;
	UPROPERTY(EditAnywhere, Category="UP Params|Mantle")
	float MantleMaxAlignmentAngle = 45.f;

	UPROPERTY(EditAnywhere, Category="UP Params|Sprint")
	float MaxSprintSpeed = 600.f;

	UPROPERTY(Transient)
	TObjectPtr<AUpCharacter> Character;
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> PlayableCharacter;

	float BaseGravityScale = 1.f;
	float BaseMaxSprintSpeed = 0.f;
	float BaseMaxWalkSpeed = 0.f;
	FRotator BaseRotationRate;

	bool bWantsToSprint = false;

	TScriptDelegate<FWeakObjectPtr> OnPlayerJumpApexReachedDelegate;
	FTimerHandle JumpCooldownTimerHandle;
	bool bCustomPressedJump = false;

	bool bHadAnimRootMotion = false;
	bool bIsPlayer = false;

	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRootMotionSource;
	uint16 TransitionRootMotionSourceId;
	bool bTransitionRootMotionSourceFinished = false;
	FUpMontageData PostTransitionMontageData;
	
	TSharedPtr<FRootMotionSource_MoveToForce> MainRootMotionSource;
	uint16 MainRootMotionSourceId;
	FVector MainRootMotionStartLocation;
	FVector MainRootMotionEndLocation;
	bool bMainRootMotionSourceFinished = false;

	FActiveGameplayEffectHandle BusyEffectHandle;
	TScriptDelegate<FWeakObjectPtr> OnAnimMontageEndedDelegate;
	uint8 MantlesMontageEndCount = 0;

	UFUNCTION()
	void AllowJump();
	UFUNCTION()
	void HandleAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnPlayerJumpApexReached() { GravityScale = JumpFallGravityScale; }

	bool IsCustomMovementModeActive(const EUpCustomMovementMode::Type InCustomMovementMode) const;

	FVector GetMantleStartLocation(const FHitResult& FrontSurfaceHit, const FHitResult& TopSurfaceHit,
		const float CapsuleHalfHeight, const float CapsuleRadius, const float MinMantleHeight, const bool bTallMantle) const;
};
