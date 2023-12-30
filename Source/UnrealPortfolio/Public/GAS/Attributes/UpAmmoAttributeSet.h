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
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MagazineFill;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, MagazineFill)

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MagazineCapacity;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, MagazineCapacity)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData AmmoReserve;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, AmmoReserve)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UUpAmmoAttributeSet, MaxAmmo)
	
	UUpAmmoAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
