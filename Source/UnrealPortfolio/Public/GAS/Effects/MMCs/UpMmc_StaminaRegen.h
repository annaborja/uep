// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UpMmc_StaminaRegen.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMmc_StaminaRegen : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UUpMmc_StaminaRegen();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
};
