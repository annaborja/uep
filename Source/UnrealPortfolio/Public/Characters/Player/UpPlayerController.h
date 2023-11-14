// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UpPlayerController.generated.h"

class AUpPlayerCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUpPlayerController();
	
	void Init() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> BaseInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> LookInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerCharacter> CustomPlayer;

	void Look(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);

	void ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting = true, const int32 Priority = 0) const;
};
