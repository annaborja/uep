// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerReputationComponent.h"

#include "UpGameInstance.h"
#include "Tags/ReputationTags.h"
#include "Utils/Structs.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool UUpPlayerReputationComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return TagSpec.Tag.MatchesTag(TAG_Reputation);
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
