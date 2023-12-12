// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UpCharacter.generated.h"

struct FUpCharacterEquipment;
struct FUpItemInstance;
class UGameplayEffect;
class UUpCharacterMovementComponent;

UENUM(BlueprintType)
namespace EUpCharacterArmingState
{
	enum Type : uint8 {
		Unarmed,
		ArmedPistol
	};
}

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;

	void SetYaw(const float InYaw);
	
	void SetRootMotionTargetLocation(const FVector& InRootMotionTargetLocation);
	void UnsetRootMotionTargetLocation();

	void EquipItem(const FUpItemInstance& ItemInstance);

	FORCEINLINE EUpCharacterArmingState::Type GetArmingState() const { return ArmingState; }
	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	
	FORCEINLINE FVector GetRootMotionTargetLocation() const { return RootMotionTargetLocation; }
	FORCEINLINE bool HasRootMotionTargetLocation() const { return bHasRootMotionTargetLocation; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;

	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
	
	UPROPERTY(Transient)
	TEnumAsByte<EUpCharacterArmingState::Type> ArmingState = EUpCharacterArmingState::Unarmed;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;
};
