// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerReputationComponent.h"

#include "Tags/ReputationTags.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Structs.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool UUpPlayerReputationComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return TagSpec.Tag.MatchesTag(TAG_Reputation);
}

UUpPlayerReputationComponent::UUpPlayerReputationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UUpPlayerReputationComponent::HandleTagSpecGrant(const FUpTagSpec& TagSpec) const
{
	if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode<AUnrealPortfolioGameModeBase>(this))
	{
		if (TagSpec.Tag.MatchesTagExact(TAG_Reputation_Affection))
		{
			return GameMode->UpdatePlayerReputation_Affection(TagSpec.RelatedTag, TagSpec.Count);
		}

		if (TagSpec.Tag.MatchesTagExact(TAG_Reputation_Esteem))
		{
			return GameMode->UpdatePlayerReputation_Esteem(TagSpec.RelatedTag, TagSpec.Count);
		}
	}

	return false;
}
