// Copyright AB. All Rights Reserved.

#include "Components/UpCombatComponent.h"

#include "Abilities/GameplayAbility.h"

bool UUpCombatComponent::IsCombatAbility(const UGameplayAbility* Ability)
{
	return false;
}

UUpCombatComponent::UUpCombatComponent()
{
	// TODO(P1): Optimize for performance (disable tick when not necessary).
	PrimaryComponentTick.bCanEverTick = true;
}
