// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPortfolioGameModeBase.h"

void AUnrealPortfolioGameModeBase::GetPlayerCharacterTags(FGameplayTagContainer& OutCharacterTags) const
{
	OutCharacterTags.AppendTags(PlayerCharacterTags);
}

bool AUnrealPortfolioGameModeBase::AddPlayerCharacterTag(const FGameplayTag& Tag)
{
	if (PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.AddTag(Tag);

	return true;
}

bool AUnrealPortfolioGameModeBase::RemovePlayerCharacterTag(const FGameplayTag& Tag)
{
	if (!PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.RemoveTag(Tag);

	return true;
}

void AUnrealPortfolioGameModeBase::GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutCharacterTags)
{
	OutCharacterTags.AppendTags(NpcCharacterTagsMap.FindOrAdd(NpcTagId));
}

bool AUnrealPortfolioGameModeBase::AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.AddTag(Tag);

	return true;
}

bool AUnrealPortfolioGameModeBase::RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (!CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.RemoveTag(Tag);

	return true;
}

void AUnrealPortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(NpcDataTable);
	check(PlayerDialogueVoice);
}
