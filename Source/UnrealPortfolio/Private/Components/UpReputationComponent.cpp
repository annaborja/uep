// Copyright AB. All Rights Reserved.

#include "Components/UpReputationComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Tags/ReputationTags.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/GeneralStructs.h"

bool UUpReputationComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return TagSpec.Tag.MatchesTag(TAG_Reputation) || TagSpec.Tag.MatchesTag(TAG_Relationship);
}

UUpReputationComponent::UUpReputationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UUpReputationComponent::HandleTagSpecGrant(const FUpTagSpec& TagSpec) const
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (TagSpec.Tag.MatchesTagExact(TAG_Reputation_Karma))
		{
			if (TagSpec.Count != 0)
			{
				GameMode->UpdatePlayerKarma(TagSpec.Count);
				return true;
			}
		}
	}

	return false;
}
