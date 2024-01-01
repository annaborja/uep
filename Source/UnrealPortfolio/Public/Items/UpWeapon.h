// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UpItem.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "UpWeapon.generated.h"

class UUpAmmoAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpWeapon : public AUpItem, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUpWeapon();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	FORCEINLINE UUpAmmoAttributeSet* GetAmmoAttributeSet() const { return AmmoAttributeSet; }
	FORCEINLINE FUpWeaponData GetWeaponData() const { return WeaponData; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitAmmoCapacityAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitAmmoAmountAttributesEffectClass;
	
	UPROPERTY()
	TObjectPtr<UUpAmmoAttributeSet> AmmoAttributeSet;

	FUpWeaponData WeaponData;
	
	virtual void BeginPlay() override;
};
