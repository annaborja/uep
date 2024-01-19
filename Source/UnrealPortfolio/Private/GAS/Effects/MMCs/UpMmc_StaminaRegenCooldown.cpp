// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_StaminaRegenCooldown.h"

#include "GAS/Attributes/UpPrimaryAttributeSet.h"

UUpMmc_StaminaRegenCooldown::UUpMmc_StaminaRegenCooldown()
{
	ResilienceDef.AttributeToCapture = UUpPrimaryAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ResilienceDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(ResilienceDef);
}

float UUpMmc_StaminaRegenCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	auto Resilience = 0.f;
	GetCapturedAttributeMagnitude(ResilienceDef, Spec, EvaluationParams, Resilience);
	Resilience = FMath::Max(Resilience, 0.f);

	return 1.f;
}
