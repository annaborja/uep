// Copyright AB. All Rights Reserved.

#include "GAS/Cues/UpCue_SpecialMoveAoeImpact.h"

#include "NiagaraFunctionLibrary.h"
#include "GAS/Abilities/UpSpecialMoveAbility_Aoe.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tags/GasTags.h"

UUpCue_SpecialMoveAoeImpact::UUpCue_SpecialMoveAoeImpact()
{
	GameplayCueTag = TAG_GameplayCue_SpecialMoveAoe_Impact;
}

bool UUpCue_SpecialMoveAoeImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const auto NormalRotation = UKismetMathLibrary::MakeRotFromX(Parameters.Normal);
	
	if (const auto Ability = Cast<UUpSpecialMoveAbility_Aoe>(Parameters.SourceObject.Get()))
	{
		if (const auto NiagaraSystem = Ability->GetVfx_Impact())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget, NiagaraSystem, Parameters.Location, NormalRotation);
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
