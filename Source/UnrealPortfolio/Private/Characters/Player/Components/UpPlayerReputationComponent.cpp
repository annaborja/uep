// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerReputationComponent.h"

#include "UpGameInstance.h"
#include "Tags/GasTags.h"
#include "Utils/Structs.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool UUpPlayerReputationComponent::IsReputationTag(const FGameplayTag& Tag)
{
	return Tag.MatchesTag(TAG_Reputation);
}

bool UUpPlayerReputationComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return IsReputationTag(TagSpec.Tag);
}

bool UUpPlayerReputationComponent::HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(WorldContextObject))
	{
		if (TagSpec.Tag.MatchesTagExact(TAG_Reputation_Affection))
		{
			return GameInstance->UpdatePlayerReputation_Affection(TagSpec.RelatedTag, TagSpec.Count);
		}

		if (TagSpec.Tag.MatchesTagExact(TAG_Reputation_Esteem))
		{
			return GameInstance->UpdatePlayerReputation_Esteem(TagSpec.RelatedTag, TagSpec.Count);
		}
	}

	return false;
}

UUpPlayerReputationComponent::UUpPlayerReputationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
