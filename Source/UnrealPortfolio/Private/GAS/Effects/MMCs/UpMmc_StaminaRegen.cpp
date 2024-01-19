// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_StaminaRegen.h"

#include "GAS/Attributes/UpPrimaryAttributeSet.h"

UUpMmc_StaminaRegen::UUpMmc_StaminaRegen()
{
	ResilienceDef.AttributeToCapture = UUpPrimaryAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ResilienceDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(ResilienceDef);
}

float UUpMmc_StaminaRegen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	auto Resilience = 0.f;
	GetCapturedAttributeMagnitude(ResilienceDef, Spec, EvaluationParams, Resilience);
	Resilience = FMath::Max(Resilience, 0.f);

	return FMath::Max(Resilience / 10.f, 1.f);
}
