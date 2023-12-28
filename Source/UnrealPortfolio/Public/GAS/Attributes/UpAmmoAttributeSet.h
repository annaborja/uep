// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UpAmmoAttributeSet.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAmmoAttributeSet : public UUpAttributeSet
{
	GENERATED_BODY()

public:
	UUpAmmoAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon1MagazineFill;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon1MagazineFill)

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon1MagazineCapacity;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon1MagazineCapacity)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon1AmmoReserve;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon1AmmoReserve)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon1MaxAmmo;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon1MaxAmmo)

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon2MagazineFill;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon2MagazineFill)

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon2MagazineCapacity;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon2MagazineCapacity)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon2AmmoReserve;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon2AmmoReserve)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Weapon2MaxAmmo;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, Weapon2MaxAmmo)
};
