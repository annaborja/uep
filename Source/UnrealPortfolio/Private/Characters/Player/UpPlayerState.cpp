// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerState.h"

#include "Tags/NpcTags.h"

AUpPlayerState::AUpPlayerState()
{
	SquadMemberTags.AddTag(TAG_Npc_Ally_A);
	SquadMemberTags.AddTag(TAG_Npc_Ally_D);
	SquadMemberTags.AddTag(TAG_Npc_Ally_F);
}

void AUpPlayerState::GetSquadMemberTags(FGameplayTagContainer& OutTags) const
{
	OutTags.AppendTags(SquadMemberTags);
}
