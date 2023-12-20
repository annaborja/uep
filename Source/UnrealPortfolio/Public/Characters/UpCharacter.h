// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Enums.h"
#include "UpCharacter.generated.h"

class AStaticMeshActor;
class UGameplayEffect;
class UUpCharacterMovementComponent;

UENUM(BlueprintType)
namespace EUpCharacterPosture
{
	enum Type : uint8 {
		Casual,
		ArmedPistol,
		ArmedRevolver,
		ArmedRifle
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
	
	virtual EUpCameraView::Type GetCameraView() const { return EUpCameraView::ThirdPerson; }
	
	FORCEINLINE EUpCharacterPosture::Type GetPosture() const { return Posture; }
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
	TObjectPtr<AStaticMeshActor> WeaponActor;
	
	UPROPERTY(Transient)
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;
};
