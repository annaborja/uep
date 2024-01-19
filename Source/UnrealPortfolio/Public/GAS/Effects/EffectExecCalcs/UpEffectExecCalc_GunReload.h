// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "UpEffectExecCalc_GunReload.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpEffectExecCalc_GunReload : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UUpEffectExecCalc_GunReload();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	FGameplayEffectAttributeCaptureDefinition AmmoReserveDef;
	FGameplayEffectAttributeCaptureDefinition MagazineCapacityDef;
	FGameplayEffectAttributeCaptureDefinition MagazineFillDef;
};
