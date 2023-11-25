// Copyright AB. All Rights Reserved.

#include "Components/UpCombatComponent.h"

#include "Abilities/GameplayAbility.h"
#include "Tags/CombatTags.h"

bool UUpCombatComponent::IsCombatAbility(const UGameplayAbility* Ability)
{
	return Ability->AbilityTags.HasTag(TAG_Combat);
}

UUpCombatComponent::UUpCombatComponent()
{
	// TODO(P1): Optimize for performance (disable tick when not necessary).
	PrimaryComponentTick.bCanEverTick = true;
}
