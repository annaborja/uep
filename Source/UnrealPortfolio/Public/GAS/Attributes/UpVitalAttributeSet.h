// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UpVitalAttributeSet.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpVitalAttributeSet : public UUpAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, Health)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, MaxHealth)

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, Shield)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, MaxShield)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, Stamina)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, MaxStamina)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UUpVitalAttributeSet, IncomingDamage);
	
	UUpVitalAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void TriggerShieldRegenCooldown(const FGameplayEffectModCallbackData& Data) const;
};
