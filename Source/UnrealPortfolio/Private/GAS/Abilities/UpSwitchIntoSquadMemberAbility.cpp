// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSwitchIntoSquadMemberAbility.h"

#include "Tags/GasTags.h"

UUpSwitchIntoSquadMemberAbility::UUpSwitchIntoSquadMemberAbility()
{
	AbilityTags.AddTag(TAG_Ability_SwitchIntoSquadMember);
	
	BlockAbilitiesWithTag.AddTag(TAG_Ability);
}
