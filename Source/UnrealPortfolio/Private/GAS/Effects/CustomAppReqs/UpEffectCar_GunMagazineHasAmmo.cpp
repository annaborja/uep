// Copyright AB. All Rights Reserved.

#include "GAS/Effects/CustomAppReqs/UpEffectCar_GunMagazineHasAmmo.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

bool UUpEffectCar_GunMagazineHasAmmo::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
                                                                    const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	float RequiredAmmo = 1.f;

	if (GameplayEffect && GameplayEffect->Modifiers.IsValidIndex(0))
	{
		if (float CalculatedMagnitude = 0.f; GameplayEffect->Modifiers[0].ModifierMagnitude.AttemptCalculateMagnitude(Spec, CalculatedMagnitude))
		{
			RequiredAmmo = FMath::Abs(CalculatedMagnitude);
		}
	}

	if (const auto Weapon = Cast<AUpWeapon>(ASC->GetAvatarActor()))
	{
		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			return AttributeSet->GetMagazineFillAttribute().GetNumericValue(AttributeSet) >= RequiredAmmo;
		}
	}
	
	return false;
}
