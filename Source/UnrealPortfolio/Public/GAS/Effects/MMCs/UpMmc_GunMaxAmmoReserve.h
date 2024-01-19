// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UpMmc_GunMaxAmmoReserve.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMmc_GunMaxAmmoReserve : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
