// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPortfolioGameModeBase.h"

#include "Characters/Player/UpPlayerState.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Tags/NpcTags.h"

AUnrealPortfolioGameModeBase::AUnrealPortfolioGameModeBase()
{
	PlayerStateClass = AUpPlayerState::StaticClass();
}

void AUnrealPortfolioGameModeBase::GetPlayerCharacterTags(FGameplayTagContainer& OutCharacterTags) const
{
	OutCharacterTags.AppendTags(PlayerCharacterTags);
}

bool AUnrealPortfolioGameModeBase::AddPlayerCharacterTag(const FGameplayTag& Tag)
{
	if (!Tag.IsValid() || PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.AddTag(Tag);
	
	return true;
}

bool AUnrealPortfolioGameModeBase::RemovePlayerCharacterTag(const FGameplayTag& Tag)
{
	if (!Tag.IsValid() || !PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.RemoveTag(Tag);
	
	return true;
}

void AUnrealPortfolioGameModeBase::GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutCharacterTags)
{
	if (!NpcTagId.IsValid() || !NpcTagId.MatchesTag(TAG_Npc)) return;
	
	OutCharacterTags.AppendTags(NpcCharacterTagsMap.FindOrAdd(NpcTagId));
}

bool AUnrealPortfolioGameModeBase::AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	if (!NpcTagId.IsValid() || !NpcTagId.MatchesTag(TAG_Npc) || !Tag.IsValid()) return false;
	
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.AddTag(Tag);
	
	return true;
}

bool AUnrealPortfolioGameModeBase::RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	if (!NpcTagId.IsValid() || !NpcTagId.MatchesTag(TAG_Npc) || !Tag.IsValid()) return false;
	
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (!CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.RemoveTag(Tag);
	
	return true;
}

FUpReputationData AUnrealPortfolioGameModeBase::GetPlayerReputationData(const FGameplayTag& TagId)
{
	if (TagId.IsValid())
	{
		if (TagId.MatchesTag(TAG_Npc)) return PlayerNpcReputationDataMap.FindOrAdd(TagId);
	}

	return FUpReputationData();
}

bool AUnrealPortfolioGameModeBase::UpdatePlayerReputation_Affection(const FGameplayTag& TagId, const int8 Delta)
{
	if (!TagId.IsValid() || Delta == 0) return false;

	if (TagId.MatchesTag(TAG_Npc))
	{
		auto& NpcReputationData = PlayerNpcReputationDataMap.FindOrAdd(TagId);
		
		NpcReputationData.Affection = FMath::Clamp(NpcReputationData.Affection + Delta,
			UUpPlayerReputationComponent::AffectionMinValue, UUpPlayerReputationComponent::AffectionMaxValue);
		
		return true;
	}

	return false;
}

bool AUnrealPortfolioGameModeBase::UpdatePlayerReputation_Esteem(const FGameplayTag& TagId, const int8 Delta)
{
	if (!TagId.IsValid() || Delta == 0) return false;
	
	if (TagId.MatchesTag(TAG_Npc))
	{
		auto& NpcReputationData = PlayerNpcReputationDataMap.FindOrAdd(TagId);
		
		NpcReputationData.Esteem = FMath::Clamp(NpcReputationData.Esteem + Delta,
			UUpPlayerReputationComponent::EsteemMinValue, UUpPlayerReputationComponent::EsteemMaxValue);
			
		return true;
	}

	return false;
}

void AUnrealPortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(NpcDataTable);
	check(PlayerDialogueVoice);
}
