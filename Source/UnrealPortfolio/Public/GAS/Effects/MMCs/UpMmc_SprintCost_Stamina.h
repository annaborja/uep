// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UpMmc_SprintCost_Stamina.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMmc_SprintCost_Stamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UUpMmc_SprintCost_Stamina();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
};
