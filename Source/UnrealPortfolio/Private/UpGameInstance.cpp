// Copyright AB. All Rights Reserved.

#include "UpGameInstance.h"

#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/Components/UpPlayerPartyComponent.h"
#include "Tags/NpcTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpGameInstance::Init()
{
	Super::Init();

	check(GasDataAsset);
	check(NpcDataTable);
	check(PlayerDialogueVoice);

	if (NpcDataTable)
	{
		TArray<FUpNpcData*> AllNpcDataRows;
		NpcDataTable->GetAllRows(TEXT("NpcDataTable GetAllRows"), AllNpcDataRows);

		for (const auto Row : AllNpcDataRows)
		{
			AllNpcData.Add(*Row);
		}
	}

	if (ItemDataTable)
	{
		TArray<FUpItemData*> AllItemDataRows;
		ItemDataTable->GetAllRows(TEXT("ItemDataTable GetAllRows"), AllItemDataRows);

		for (const auto Row : AllItemDataRows)
		{
			AllItemData.Add(*Row);
		}
	}
}

FUpNpcData UUpGameInstance::GetNpcData(const FGameplayTag& NpcTagId)
{
	FUpNpcData Result;

	if (UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("GetNpcData")))  
	{
		for (const auto Data : AllNpcData)
		{
			if (Data.TagId.MatchesTagExact(NpcTagId))
			{
				Result = Data;
			}
		}
	}

	return Result;
}

void UUpGameInstance::GetPlayerCharacterTags(FGameplayTagContainer& OutTags) const
{
	OutTags.AppendTags(PlayerCharacterTags);
}

bool UUpGameInstance::AddPlayerCharacterTag(const FGameplayTag& Tag)
{
	if (!UUpBlueprintFunctionLibrary::ValidateTag(Tag, TEXT("AddPlayerCharacterTag"))) return false;
	
	if (PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.AddTag(Tag);
	
	return true;
}

bool UUpGameInstance::RemovePlayerCharacterTag(const FGameplayTag& Tag)
{
	if (!UUpBlueprintFunctionLibrary::ValidateTag(Tag, TEXT("RemovePlayerCharacterTag"))) return false;
	
	if (!PlayerCharacterTags.HasTagExact(Tag)) return false;

	PlayerCharacterTags.RemoveTag(Tag);
	
	return true;
}

void UUpGameInstance::GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutTags)
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("GetNpcCharacterTags"))) return;
	
	OutTags.AppendTags(NpcCharacterTagsMap.FindOrAdd(NpcTagId));
}

bool UUpGameInstance::AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("AddNpcCharacterTag"))) return false;
	if (!UUpBlueprintFunctionLibrary::ValidateTag(Tag, TEXT("AddNpcCharacterTag"))) return false;
	
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.AddTag(Tag);
	
	return true;
}

bool UUpGameInstance::RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag)
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("RemoveNpcCharacterTag"))) return false;
	if (!UUpBlueprintFunctionLibrary::ValidateTag(Tag, TEXT("RemoveNpcCharacterTag"))) return false;
	
	auto& CharacterTags = NpcCharacterTagsMap.FindOrAdd(NpcTagId);
	
	if (!CharacterTags.HasTagExact(Tag)) return false;

	CharacterTags.RemoveTag(Tag);
	
	return true;
}

FUpPartyMembershipSpec UUpGameInstance::GetPartyMembershipSpec(const FGameplayTag& TagId) const
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(TagId, TEXT("GetPartyMembershipSpec"))) return FUpPartyMembershipSpec();

	if (const auto PartyMembershipSpec = PartyMembershipSpecMap.Find(TagId))
	{
		return *PartyMembershipSpec;
	}
	
	return FUpPartyMembershipSpec();
}

void UUpGameInstance::GetPartyMemberTags(FGameplayTagContainer& OutTags) const
{
	OutTags.AppendTags(PartyMemberTags);
}

void UUpGameInstance::AddPartyMember(const FGameplayTag& NpcTagId)
{
	UE_LOG(LogTemp, Warning, TEXT("Add party member %s"), *NpcTagId.ToString())
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("AddPartyMember"))) return;
	
	PartyMemberTags.AddTag(NpcTagId);
}

void UUpGameInstance::RemovePartyMember(const FGameplayTag& NpcTagId)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove party member %s"), *NpcTagId.ToString())
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("RemovePartyMember"))) return;
	
	PartyMemberTags.RemoveTag(NpcTagId);
}

FUpReputationData UUpGameInstance::GetPlayerReputationData(const FGameplayTag& TagId)
{
	if (!UUpBlueprintFunctionLibrary::ValidateTag(TagId, TEXT("GetPlayerReputationData"))) return FUpReputationData();
	
	if (TagId.MatchesTag(TAG_Npc)) return PlayerNpcReputationDataMap.FindOrAdd(TagId);

	UE_LOG(LogTemp, Error, TEXT("GetPlayerReputationData: Unmatched tag ID %s"), *TagId.ToString())
	return FUpReputationData();
}

bool UUpGameInstance::UpdatePlayerReputation_Affection(const FGameplayTag& TagId, const int8 Delta)
{
	if (!UUpBlueprintFunctionLibrary::ValidateTag(TagId, TEXT("UpdatePlayerReputation_Affection"))) return false;
	
	if (Delta == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerReputation_Affection: Delta == 0 for tag ID %s"), *TagId.ToString())
		return false;
	}

	if (TagId.MatchesTag(TAG_Npc))
	{
		auto& NpcReputationData = PlayerNpcReputationDataMap.FindOrAdd(TagId);
		
		NpcReputationData.Affection = FMath::Clamp(NpcReputationData.Affection + Delta,
			UUpPlayerReputationComponent::AffectionMinValue, UUpPlayerReputationComponent::AffectionMaxValue);
		
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("UpdatePlayerReputation_Affection: Unmatched tag ID %s"), *TagId.ToString())
	return false;
}

bool UUpGameInstance::UpdatePlayerReputation_Esteem(const FGameplayTag& TagId, const int8 Delta)
{
	if (!UUpBlueprintFunctionLibrary::ValidateTag(TagId, TEXT("UpdatePlayerReputation_Esteem"))) return false;
	
	if (Delta == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerReputation_Esteem: Delta == 0 for tag ID %s"), *TagId.ToString())
		return false;
	}
	
	if (TagId.MatchesTag(TAG_Npc))
	{
		auto& NpcReputationData = PlayerNpcReputationDataMap.FindOrAdd(TagId);
		
		NpcReputationData.Esteem = FMath::Clamp(NpcReputationData.Esteem + Delta,
			UUpPlayerReputationComponent::EsteemMinValue, UUpPlayerReputationComponent::EsteemMaxValue);
			
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("UpdatePlayerReputation_Esteem: Unmatched tag ID %s"), *TagId.ToString())
	return false;
}

FUpItemData UUpGameInstance::GetItemData(const FGameplayTag& ItemTagId)
{
	FUpItemData Result;

	if (UUpBlueprintFunctionLibrary::ValidateItemTag(ItemTagId, TEXT("GetItemData")))  
	{
		for (const auto Data : AllItemData)
		{
			if (Data.TagId.MatchesTagExact(ItemTagId))
			{
				Result = Data;
			}
		}
	}

	return Result;
}

FUpInventory UUpGameInstance::GetNpcInventory(const FGameplayTag& NpcTagId)
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("GetNpcInventory"))) return FUpInventory();
	
	return NpcInventoryMap.FindOrAdd(NpcTagId);
}
