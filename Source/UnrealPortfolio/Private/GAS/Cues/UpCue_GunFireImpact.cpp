// Copyright AB. All Rights Reserved.

#include "GAS/Cues/UpCue_GunFireImpact.h"

#include "NiagaraFunctionLibrary.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tags/GasTags.h"

UUpCue_GunFireImpact::UUpCue_GunFireImpact()
{
	GameplayCueTag = TAG_GameplayCue_GunFire_Impact;
}

bool UUpCue_GunFireImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const auto NormalRotation = UKismetMathLibrary::MakeRotFromX(Parameters.Normal);
	
	if (const auto Weapon = Cast<AUpWeapon>(Parameters.EffectCauser.Get()))
	{
		if (const auto HitResultPtr = Parameters.EffectContext.GetHitResult())
		{
			const auto SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResultPtr->PhysMaterial.Get());
			const auto SoundPtr = Weapon->GetSfx_Impacts().Find(SurfaceType);

			if (const auto Sound = SoundPtr == nullptr ? nullptr : *SoundPtr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Sound, Parameters.Location, NormalRotation,
					Weapon->GetTagId().MatchesTag(TAG_Item_Weapon_Shotgun)? 0.5f : 0.8f);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("No bullet impact sound for surface type %d"), SurfaceType)
			}
		}

		if (const auto NiagaraSystem = Weapon->GetNiagaraSystem_Impact())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(Weapon, NiagaraSystem, Parameters.Location, NormalRotation);
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
