// Copyright AB. All Rights Reserved.

#include "UpGameInstance.h"

#include "Characters/UpCharacter.h"
#include "Characters/Player/Components/UpPlayerPartyComponent.h"
#include "GAS/UpGasDataAsset.h"
#include "Tags/NpcTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpGameInstance::Init()
{
	Super::Init();

	check(CharacterDataTable);
	check(GasDataAsset);
	check(HitBoxDataTable);
	check(ItemDataTable);
	check(MissionDataTable);
	check(ObjectiveDataTable);
	check(PlayerDialogueVoice);
	check(SpecialMoveDataTable);
	check(WeaponDataTable);

	TArray<FUpCharacterData*> AllCharacterDataRows;
	CharacterDataTable->GetAllRows(TEXT("CharacterDataTable GetAllRows"), AllCharacterDataRows);

	for (const auto Row : AllCharacterDataRows)
	{
		AllCharacterData.Add(*Row);
	}

	TArray<FUpItemData*> AllItemDataRows;
	ItemDataTable->GetAllRows(TEXT("ItemDataTable GetAllRows"), AllItemDataRows);

	for (const auto Row : AllItemDataRows)
	{
		AllItemData.Add(*Row);
	}

	TArray<FUpMissionData*> AllMissionDataRows;
	MissionDataTable->GetAllRows(TEXT("MissionDataTable GetAllRows"), AllMissionDataRows);

	for (const auto Row : AllMissionDataRows)
	{
		AllMissionData.Add(*Row);
	}

	TArray<FUpObjectiveData*> AllObjectiveDataRows;
	ObjectiveDataTable->GetAllRows(TEXT("ObjectiveDataTable GetAllRows"), AllObjectiveDataRows);

	for (const auto Row : AllObjectiveDataRows)
	{
		AllObjectiveData.Add(*Row);
	}

	TArray<FUpSpecialMoveData*> AllSpecialMoveDataRows;
	SpecialMoveDataTable->GetAllRows(TEXT("SpecialMoveDataTable GetAllRows"), AllSpecialMoveDataRows);

	for (const auto Row : AllSpecialMoveDataRows)
	{
		AllSpecialMoveData.Add(*Row);
	}

	TArray<FUpWeaponData*> AllWeaponDataRows;
	WeaponDataTable->GetAllRows(TEXT("WeaponDataTable GetAllRows"), AllWeaponDataRows);

	for (const auto Row : AllWeaponDataRows)
	{
		AllWeaponData.Add(*Row);
	}
}

FUpCharacterData UUpGameInstance::GetCharacterData(const FGameplayTag& NpcTagId)
{
	FUpCharacterData Result;

	if (UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("GetCharacterData")))  
	{
		for (const auto Data : AllCharacterData)
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

FUpMissionData UUpGameInstance::GetMissionData(const FGameplayTag& TagId)
{
	FUpMissionData Result;

	for (const auto Data : AllMissionData)
	{
		if (Data.TagId.MatchesTagExact(TagId))
		{
			Result = Data;
		}
	}

	return Result;
}

FUpObjectiveData UUpGameInstance::GetObjectiveData(const FGameplayTag& TagId)
{
	FUpObjectiveData Result;

	for (const auto Data : AllObjectiveData)
	{
		if (Data.TagId.MatchesTagExact(TagId))
		{
			Result = Data;
		}
	}

	return Result;
}

FUpInventory UUpGameInstance::GetNpcInventory(const FGameplayTag& NpcTagId)
{
	if (!UUpBlueprintFunctionLibrary::ValidateNpcTag(NpcTagId, TEXT("GetNpcInventory"))) return FUpInventory();
	
	return NpcInventoryMap.FindOrAdd(NpcTagId);
}

FUpWeaponData UUpGameInstance::GetWeaponData(const FGameplayTag& WeaponTagId)
{
	FUpWeaponData Result;

	if (UUpBlueprintFunctionLibrary::ValidateWeaponTag(WeaponTagId, TEXT("GetWeaponData")))  
	{
		for (const auto Data : AllWeaponData)
		{
			if (Data.TagId.MatchesTagExact(WeaponTagId))
			{
				Result = Data;
			}
		}
	}

	return Result;
}

FUpSpecialMoveData UUpGameInstance::GetSpecialMoveData(const FGameplayTag& TagId)
{
	FUpSpecialMoveData Result;

	for (const auto Data : AllSpecialMoveData)
	{
		if (Data.TagId.MatchesTagExact(TagId))
		{
			Result = Data;
		}
	}

	return Result;
}

void UUpGameInstance::ApplyBusyState(AUpCharacter* Character) const
{
	if (!GasDataAsset) return;

	if (const auto AbilitySystemComponent = Character->GetAbilitySystemComponent())
	{
		if (const auto EffectClass = GasDataAsset->GetEffectClass_BusyState())
		{
			Character->SetEffectHandle_BusyState(AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, AbilitySystemComponent->MakeEffectContext()).Data.Get()));
		}
	}
}

void UUpGameInstance::RemoveBusyState(AUpCharacter* Character) const
{
	const auto& EffectHandle = Character->GetEffectHandle_BusyState();
	
	if (!EffectHandle.IsValid()) return;

	if (const auto AbilitySystemComponent = Character->GetAbilitySystemComponent())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
	}
	
	Character->SetEffectHandle_BusyState(FActiveGameplayEffect().Handle);
}
