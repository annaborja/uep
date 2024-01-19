// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "UpEffectCar_StaminaRegen.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpEffectCar_StaminaRegen : public UGameplayEffectCustomApplicationRequirement
{
	GENERATED_BODY()

public:
	virtual bool CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
		const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const override;
};
