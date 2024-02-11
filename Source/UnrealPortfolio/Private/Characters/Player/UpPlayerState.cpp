// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerState.h"

#include "UpGameInstance.h"
#include "Tags/NpcTags.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

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

void AUpPlayerState::ProgressMission(const FGameplayTag& TagId)
{
	uint8 NextStep = 0;
	
	if (MissionMap.Contains(TagId))
	{
		NextStep = MissionMap.Find(TagId)->Step + 1;
	}

	MissionMap.FindOrAdd(TagId).Step = NextStep;
	
	TArray<FString> TagSegments;
	TagId.ToString().ParseIntoArray(TagSegments, TEXT("."));

	if (!TagSegments.IsValidIndex(1)) return;

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto ObjectiveTag = FGameplayTag::RequestGameplayTag(
			FName(TEXT("Objective.") + TagSegments[1] + TEXT(".") + FString::FromInt(NextStep))); ObjectiveTag.IsValid())
		{
			if (const auto& ObjectiveData = GameInstance->GetObjectiveData(ObjectiveTag); ObjectiveData.IsValid())
			{
				if (const auto CustomHud = UUpBlueprintFunctionLibrary::GetCustomHud(this))
				{
					CustomHud->BroadcastObjective(ObjectiveData);
				}
			}
		}
	}
}
