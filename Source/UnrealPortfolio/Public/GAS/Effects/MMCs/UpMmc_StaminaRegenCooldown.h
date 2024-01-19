// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UpMmc_StaminaRegenCooldown.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMmc_StaminaRegenCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UUpMmc_StaminaRegenCooldown();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
};
