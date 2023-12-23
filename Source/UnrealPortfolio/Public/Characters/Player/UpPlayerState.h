// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "UpPlayerState.generated.h"

class UUpAttributeSet;
class UUpHealthAttributeSet;
class UUpPrimaryAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUpPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	TArray<UUpAttributeSet*> GetAttributeSets() const;

	FORCEINLINE UUpHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; }
	FORCEINLINE UUpPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAttributeSet; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UUpHealthAttributeSet> HealthAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
};
