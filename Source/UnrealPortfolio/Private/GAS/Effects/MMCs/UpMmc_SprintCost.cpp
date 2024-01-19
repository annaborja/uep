// Copyright AB. All Rights Reserved.

#include "GAS/Effects/MMCs/UpMmc_SprintCost.h"

#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"

UUpMmc_SprintCost::UUpMmc_SprintCost()
{
	ResilienceDef.AttributeToCapture = UUpPrimaryAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ResilienceDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(ResilienceDef);
}

float UUpMmc_SprintCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const auto EffectSpecHandle = Spec.GetContext();
	
	if (const auto Character = Cast<AUpCharacter>(EffectSpecHandle.GetInstigator()))
	{
		if (const auto CustomMovementComponent = Character->GetCustomMovementComponent();
			Character->GetHorizontalSpeed() <= 0.f || (CustomMovementComponent && CustomMovementComponent->IsFalling()))
		{
			return 0.f;
		}
	}
	
	const auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	auto Resilience = 0.f;
	GetCapturedAttributeMagnitude(ResilienceDef, Spec, EvaluationParams, Resilience);
	Resilience = FMath::Max(Resilience, 0.f);

	return FMath::Min(-10.f + Resilience / 10.f, -1.f);
}
