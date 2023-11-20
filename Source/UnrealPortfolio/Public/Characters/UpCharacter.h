// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UpCharacter.generated.h"

class UUpCharacterMovementComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;

	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;
	
	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
};
