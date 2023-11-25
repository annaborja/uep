// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/UpCombatable.h"
#include "UpCharacter.generated.h"

class UGameplayEffect;
class UUpCharacterMovementComponent;
class UUpCombatComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter, public IUpCombatable
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;

	virtual UUpCombatComponent* GetCombatComponent() const override { return CombatComponent; }

	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;

	UPROPERTY(Transient)
	TObjectPtr<UUpCombatComponent> CombatComponent;
	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
};
