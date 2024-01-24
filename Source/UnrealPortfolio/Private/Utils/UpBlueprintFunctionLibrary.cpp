// Copyright AB. All Rights Reserved.

#include "Utils/UpBlueprintFunctionLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/Components/UpPlayerPartyComponent.h"
#include "Items/UpItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Tags/ItemTags.h"
#include "Tags/NpcTags.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Constants.h"
#include "Utils/Structs.h"

AUpHud* UUpBlueprintFunctionLibrary::GetCustomHud(const UObject* WorldContextObject)
{
	if (const auto PlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		return PlayerController->GetCustomHud();
	}

	return nullptr;
}

UUpGameInstance* UUpBlueprintFunctionLibrary::GetGameInstance(const UObject* WorldContextObject)
{
	return Cast<UUpGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

AUnrealPortfolioGameModeBase* UUpBlueprintFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
}

FText UUpBlueprintFunctionLibrary::GetInGameName(const UObject* WorldContextObject, const FGameplayTag& TagId)
{
	if (ValidateTag(TagId, TEXT("GetInGameName")))
	{
		if (const auto GameInstance = GetGameInstance(WorldContextObject))
		{
			if (TagId.MatchesTag(TAG_Npc))
			{
				if (const auto CharacterData = GameInstance->GetCharacterData(TagId); CharacterData.IsValid())
				{
					return CharacterData.Name;
				}
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("GetInGameName: No data found for tag ID %s"), *TagId.ToString())
	
	return FText::FromString(TagId.ToString());
}

FText UUpBlueprintFunctionLibrary::GetInGameNameifiedText(const UObject* WorldContextObject, const FText InText)
{
	auto Result = FString(InText.ToString());
	
	if (const auto GameInstance = GetGameInstance(WorldContextObject))
	{
		if (const auto CharacterDataTable = GameInstance->GetCharacterDataTable())
		{
			FRegexMatcher Matcher(FRegexPattern(TEXT("\\[.+?\\]")), InText.ToString());
			TArray<FUpCharacterData*> AllCharacterDataRows;
			uint8 i = 0;
	
			while (Matcher.FindNext())
			{
				if (++i > 100)
				{
					UE_LOG(LogTemp, Error, TEXT("GetInGameNameifiedText: Infinite loop break hit"))
					break;
				}

				if (AllCharacterDataRows.Num() <= 0)
				{
					CharacterDataTable->GetAllRows(TEXT("CharacterDataTable GetAllRows"), AllCharacterDataRows);
				}
		
				auto CaptureGroup = Matcher.GetCaptureGroup(0);
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "[", "");
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "]", "");

				FUpCharacterData CharacterData;

				for (const auto Row : AllCharacterDataRows)
				{
					if (Row->TagId.ToString() == CaptureGroup)
					{
						CharacterData = *Row;
						break;
					}
				}

				if (CharacterData.IsValid())
				{
					UKismetStringLibrary::ReplaceInline(Result, "[" + CaptureGroup + "]", CharacterData.Name.ToString());
				} else
				{
					UE_LOG(LogTemp, Error, TEXT("GetInGameNameifiedText: No NPC data found for capture group %s"), *CaptureGroup)
				}
			}
		}
	}
	
	return FText::FromString(Result);
}

// UE sometimes appends a numeric suffix (`_#`) to gameplay tags (possibly a UE bug?).
// This function normalizes these tags.
FGameplayTag UUpBlueprintFunctionLibrary::GetNormalizedTag(const FGameplayTag& Tag)
{
	TArray<FString> TagSegments;
	Tag.ToString().ParseIntoArray(TagSegments, TEXT("_"));

	return FGameplayTag::RequestGameplayTag(FName(TagSegments[0]));
}

bool UUpBlueprintFunctionLibrary::HasTag(const AActor* Actor, const FGameplayTag& Tag)
{
	if (!ValidateTag(Tag, TEXT("HasTag"))) return false;
	
	if (const auto GameplayTagAsset = Cast<IGameplayTagAssetInterface>(Actor))
	{
		FGameplayTagContainer TagContainer;
		GameplayTagAsset->GetOwnedGameplayTags(TagContainer);

		return TagContainer.HasTagExact(Tag);
	}

	return false;
}

bool UUpBlueprintFunctionLibrary::HasTagId(const AActor* Actor, const FGameplayTag& Tag)
{
	if (!ValidateTag(Tag, TEXT("HasTagId"))) return false;
	
	if (const auto TagIdable = Cast<IUpTagIdable>(Actor))
	{
		return TagIdable->GetTagId().MatchesTagExact(Tag);
	}

	return false;
}

bool UUpBlueprintFunctionLibrary::ValidateTag(const FGameplayTag& Tag, const FString FuncName)
{
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Invalid tag %s"), *FuncName, *Tag.ToString())
		return false;
	}
	
	return true;
}

bool UUpBlueprintFunctionLibrary::ValidateItemTag(const FGameplayTag& Tag, const FString FuncName)
{
	if (!ValidateTag(Tag, FuncName)) return false;

	if (!Tag.MatchesTag(TAG_Item))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Non-item tag %s"), *FuncName, *Tag.ToString())
		return false;
	}
		
	return true;
}

bool UUpBlueprintFunctionLibrary::ValidateNpcTag(const FGameplayTag& Tag, const FString FuncName)
{
	if (!ValidateTag(Tag, FuncName)) return false;

	if (!Tag.MatchesTag(TAG_Npc))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Non-NPC tag %s"), *FuncName, *Tag.ToString())
		return false;
	}
		
	return true;
}

bool UUpBlueprintFunctionLibrary::ValidateWeaponTag(const FGameplayTag& Tag, const FString FuncName)
{
	if (!ValidateTag(Tag, FuncName)) return false;

	if (!Tag.MatchesTag(TAG_Item_Weapon))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Non-weapon tag %s"), *FuncName, *Tag.ToString())
		return false;
	}
		
	return true;	
}

FString UUpBlueprintFunctionLibrary::GetWeaponMontageSectionName(const AUpCharacter* Character)
{
	if (Character->IsRelaxed()) return TEXT("Unarmed");
	
	switch (Character->GetPosture())
	{
	case EUpCharacterPosture::ArmedPistol:
		return TEXT("ArmedPistol");
	case EUpCharacterPosture::ArmedRevolver:
		return TEXT("ArmedRevolver");
	case EUpCharacterPosture::ArmedRifle:
		return TEXT("ArmedRifle");
	default:
		return TEXT("Unarmed");
	}
}

bool UUpBlueprintFunctionLibrary::IsTwoHandedGunTag(const FGameplayTag& Tag)
{
	FGameplayTagContainer TwoHandedGunTags;
	TwoHandedGunTags.AddTag(TAG_Item_Weapon_AssaultRifle);
	TwoHandedGunTags.AddTag(TAG_Item_Weapon_Shotgun);
	TwoHandedGunTags.AddTag(TAG_Item_Weapon_SniperRifle);
	TwoHandedGunTags.AddTag(TAG_Item_Weapon_SubmachineGun);

	return TwoHandedGunTags.HasTag(Tag);
}

bool UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition)
{
	if (const auto GameInstance = GetGameInstance(WorldContextObject))
	{
		if (EntityTagSpec.PlayerTagSpecs.Num() > 0)
		{
			FGameplayTagContainer PlayerCharacterTags;
			GameInstance->GetPlayerCharacterTags(PlayerCharacterTags);
			
			for (const auto& PlayerTagSpec : EntityTagSpec.PlayerTagSpecs)
			{
				if (!ValidateTagSpec(PlayerTagSpec, TEXT("IsEntityTagSpecSatisfied"))) return false;
				
				if (bProhibition && PlayerCharacterTags.HasTagExact(PlayerTagSpec.Tag)) return false;
				if (!bProhibition && !PlayerCharacterTags.HasTagExact(PlayerTagSpec.Tag)) return false;
			}
		}
		
		for (const auto& NpcTagToTagSpecMappings : EntityTagSpec.NpcTagSpecMappings)
		{
			const auto NpcTagId = NpcTagToTagSpecMappings.Tag;

			if (!ValidateNpcTag(NpcTagId, TEXT("IsEntityTagSpecSatisfied"))) return false;
			
			FGameplayTagContainer NpcCharacterTags;
			GameInstance->GetNpcCharacterTags(NpcTagId, NpcCharacterTags);
			
			for (const auto& NpcTagSpec : NpcTagToTagSpecMappings.TagSpecs)
			{
				if (!ValidateTagSpec(NpcTagSpec, TEXT("IsEntityTagSpecSatisfied"))) return false;
				
				if (bProhibition && NpcCharacterTags.HasTagExact(NpcTagSpec.Tag)) return false;
				if (!bProhibition && !NpcCharacterTags.HasTagExact(NpcTagSpec.Tag)) return false;
			}
		}
		
		// TODO(P0): Finish implementation.

		return true;
	}

	return false;
}

void UUpBlueprintFunctionLibrary::ProcessEntityTagSpecGrants(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec)
{
	for (const auto& PlayerTagSpec : EntityTagSpec.PlayerTagSpecs)
	{
		if (!ValidateTagSpec(PlayerTagSpec, TEXT("ProcessEntityTagSpecGrants"))) continue;
		
		GrantPlayerTagSpec(WorldContextObject, PlayerTagSpec);
	}

	for (const auto& NpcTagToTagSpecMappings : EntityTagSpec.NpcTagSpecMappings)
	{
		const auto NpcTagId = NpcTagToTagSpecMappings.Tag;

		if (!ValidateNpcTag(NpcTagId, TEXT("ProcessEntityTagSpecGrants"))) continue;
		
		for (const auto& NpcTagSpec : NpcTagToTagSpecMappings.TagSpecs)
		{
			if (!ValidateTagSpec(NpcTagSpec, TEXT("ProcessEntityTagSpecGrants"))) continue;
			
			GrantNpcTagSpec(WorldContextObject, NpcTagId, NpcTagSpec);
		}
	}

	// TODO(P0): Finish implementation.
}

bool UUpBlueprintFunctionLibrary::ValidateTagSpec(const FUpTagSpec& TagSpec, const FString FuncName)
{
	if (!TagSpec.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Invalid tag spec %s (%d)"), *FuncName, *TagSpec.Tag.ToString(), TagSpec.Count)
		return false;
	}

	return true;
}

void UUpBlueprintFunctionLibrary::GrantNpcTagSpec(const UObject* WorldContextObject, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec)
{
	if (!ValidateNpcTag(NpcTagId, TEXT("GrantNpcTagSpec"))) return;
	if (!ValidateTagSpec(TagSpec, TEXT("GrantNpcTagSpec"))) return;
	
	if (const auto GameInstance = GetGameInstance(WorldContextObject))
	{
		if (GameInstance->ShouldDebugTagSpecGrant())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s GrantTagSpec: %s (%d)"), *NpcTagId.ToString(), *TagSpec.Tag.ToString(), TagSpec.Count)
		}
		
		bool bSuccess = false;
		
		if (TagSpec.Count > 0)
		{
			bSuccess = GameInstance->AddNpcCharacterTag(NpcTagId, TagSpec.Tag);
		} else if (TagSpec.Count < 0)
		{
			bSuccess = GameInstance->RemoveNpcCharacterTag(NpcTagId, TagSpec.Tag);
		}
	}
}

void UUpBlueprintFunctionLibrary::GrantPlayerTagSpec(const UObject* WorldContextObject, const FUpTagSpec& TagSpec)
{
	if (!ValidateTagSpec(TagSpec, TEXT("GrantPlayerTagSpec"))) return;
	
	if (const auto GameInstance = GetGameInstance(WorldContextObject))
	{
		if (GameInstance->ShouldDebugTagSpecGrant())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player GrantTagSpec: %s (%d)"), *TagSpec.Tag.ToString(), TagSpec.Count)
		}
		
		bool bSuccess = false;
		
		if (UUpPlayerReputationComponent::ShouldHandleTagSpecGrant(TagSpec))
		{
			bSuccess = UUpPlayerReputationComponent::HandleTagSpecGrant(WorldContextObject, TagSpec);
		} else if (TagSpec.Count > 0)
		{
			bSuccess = GameInstance->AddPlayerCharacterTag(TagSpec.Tag);
		} else if (TagSpec.Count < 0)
		{
			bSuccess = GameInstance->RemovePlayerCharacterTag(TagSpec.Tag);
		}

		if (bSuccess)
		{
			UUpPlayerPartyComponent::OnPlayerTagSpecGranted(WorldContextObject, TagSpec);
		}
	}
}

FVector UUpBlueprintFunctionLibrary::CalculateVelocity(const FVector& FromLocation, const FVector& ToLocation, const float Duration, const float GravityScale)
{
	return FVector(
	(ToLocation.X - FromLocation.X) / Duration,
	(ToLocation.Y - FromLocation.Y) / Duration,
	(ToLocation.Z - FromLocation.Z - FMath::Pow(Duration, 2) * 0.5f * GRAVITATIONAL_ACCELERATION * GravityScale) / Duration);
}

bool UUpBlueprintFunctionLibrary::HasTargetBeenReached(const float CurrentVal, const float OldVal, const float TargetVal)
{
	if (OldVal > TargetVal) return CurrentVal <= TargetVal;

	return CurrentVal >= TargetVal;
}

void UUpBlueprintFunctionLibrary::TearDownActorComponent(UActorComponent* ActorComponent)
{
	if (IsValid(ActorComponent))
	{
		ActorComponent->UnregisterComponent();
		ActorComponent->DestroyComponent();
	}
}
