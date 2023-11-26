// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/Structs.h"
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
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	void ToggleSprint(const bool bInWantsToSprint) { bWantsToSprint = bInWantsToSprint; }
	bool TryMantle();

	void ResetMaxWalkSpeed() { MaxWalkSpeed = BaseMaxWalkSpeed; }
	void ResetRotationRate() { RotationRate = BaseRotationRate; }
	
	FORCEINLINE float GetMaxSprintSpeed() const { return MaxSprintSpeed; }
	FORCEINLINE bool IsSprinting() const { return bWantsToSprint; }
	
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMantle = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;

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
	float MaxSprintSpeed = 700.f;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerCharacter> Player;
	
	float BaseGravityScale = 1.f;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UAnimMontage> MantlesMontage;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpCharacter> Character;
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;
	
	float BaseMaxWalkSpeed = 0.f;
	FRotator BaseRotationRate;
	
	bool bHadAnimRootMotion = false;
	bool bWantsToSprint = false;

	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRootMotionSource;
	uint16 TransitionRootMotionSourceId;
	bool bTransitionRootMotionSourceFinished = false;
	FUpMontageData PostTransitionMontageData;

	bool IsCustomMovementModeActive(const EUpCustomMovementMode::Type InCustomMovementMode) const;
	
	FVector GetMantleStartLocation(const FHitResult& FrontSurfaceHit, const FHitResult& TopSurfaceHit,
		const float CapsuleHalfHeight, const float CapsuleRadius, const float MinMantleHeight, const bool bTallMantle) const;
};
