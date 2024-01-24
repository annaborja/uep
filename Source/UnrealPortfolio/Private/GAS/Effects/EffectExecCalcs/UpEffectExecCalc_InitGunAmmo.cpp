// Copyright AB. All Rights Reserved.

#include "GAS/Effects/EffectExecCalcs/UpEffectExecCalc_InitGunAmmo.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"

void UUpEffectExecCalc_InitGunAmmo::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                           FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (const auto TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent())
	{
		if (const auto Weapon = Cast<AUpWeapon>(TargetAbilitySystemComponent->GetAvatarActor()))
		{
			const auto MaxAmmo = Weapon->GetMaxAmmo();
			const auto MagazineCapacity = Weapon->GetMagazineCapacity();
		
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetMaxAmmoAttribute(), EGameplayModOp::Override, MaxAmmo));
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetMagazineCapacityAttribute(), EGameplayModOp::Override, MagazineCapacity));
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetMagazineFillAttribute(), EGameplayModOp::Override, MagazineCapacity));
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(UUpAmmoAttributeSet::GetAmmoReserveAttribute(), EGameplayModOp::Override, MaxAmmo - MagazineCapacity));
		}
	}
}
