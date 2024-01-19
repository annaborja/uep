// Copyright AB. All Rights Reserved.

#include "GAS/Effects/EffectExecCalcs/UpEffectExecCalc_GunReload.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"

UUpEffectExecCalc_GunReload::UUpEffectExecCalc_GunReload()
{
	AmmoReserveDef.AttributeToCapture = UUpAmmoAttributeSet::GetAmmoReserveAttribute();
	AmmoReserveDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AmmoReserveDef.bSnapshot = false;
	
	MagazineCapacityDef.AttributeToCapture = UUpAmmoAttributeSet::GetMagazineCapacityAttribute();
	MagazineCapacityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagazineCapacityDef.bSnapshot = false;
	
	MagazineFillDef.AttributeToCapture = UUpAmmoAttributeSet::GetMagazineFillAttribute();
	MagazineFillDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagazineFillDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AmmoReserveDef);
	RelevantAttributesToCapture.Add(MagazineCapacityDef);
	RelevantAttributesToCapture.Add(MagazineFillDef);
}

void UUpEffectExecCalc_GunReload::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const auto& EffectSpec = ExecutionParams.GetOwningSpec();
	const auto SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float AmmoReserve = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AmmoReserveDef, EvaluationParams, AmmoReserve);
	
	float MagazineCapacity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagazineCapacityDef, EvaluationParams, MagazineCapacity);
	
	float MagazineFill = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagazineFillDef, EvaluationParams, MagazineFill);

	const auto ReloadAmount = FMath::Min(MagazineCapacity - MagazineFill, AmmoReserve);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetAmmoReserveAttribute(), EGameplayModOp::Override, AmmoReserve - ReloadAmount));
	
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetMagazineFillAttribute(), EGameplayModOp::Override, MagazineFill + ReloadAmount));
}
