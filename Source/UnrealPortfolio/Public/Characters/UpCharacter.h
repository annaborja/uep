// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/UpInventoriable.h"
#include "UpCharacter.generated.h"

class UGameplayEffect;
class UUpCharacterMovementComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter, public IUpInventoriable
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;

	virtual UUpInventoryComponent* GetInventoryComponent() const override { return InventoryComponent; }

	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;

	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
};
