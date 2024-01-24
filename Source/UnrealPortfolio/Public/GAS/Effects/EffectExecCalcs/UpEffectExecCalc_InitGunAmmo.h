// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "UpEffectExecCalc_InitGunAmmo.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpEffectExecCalc_InitGunAmmo : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
