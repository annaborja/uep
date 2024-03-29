// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "UpEffectCar_AmmoGrant.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpEffectCar_AmmoGrant : public UGameplayEffectCustomApplicationRequirement
{
	GENERATED_BODY()

public:
	virtual bool CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
		const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const override;
};
