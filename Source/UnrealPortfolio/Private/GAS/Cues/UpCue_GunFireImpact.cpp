// Copyright AB. All Rights Reserved.

#include "GAS/Cues/UpCue_GunFireImpact.h"

#include "NiagaraFunctionLibrary.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Tags/GasTags.h"

UUpCue_GunFireImpact::UUpCue_GunFireImpact()
{
	GameplayCueTag = TAG_GameplayCue_GunFire_Impact;
}

bool UUpCue_GunFireImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const auto NormalRotation = UKismetMathLibrary::MakeRotFromX(Parameters.Normal);
	
	if (const auto Weapon = Cast<AUpWeapon>(Parameters.SourceObject.Get()))
	{
		if (const auto NiagaraSystem = Weapon->GetNiagaraSystem_Impact())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(Weapon, NiagaraSystem, Parameters.Location, NormalRotation);
		}
	}
	
	if (const auto Character = Cast<AUpCharacter>(MyTarget))
	{
		if (const auto Sfx = Character->GetSfx_BulletImpacts())
		{
			UGameplayStatics::PlaySoundAtLocation(Character, Sfx, Parameters.Location, NormalRotation);
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
