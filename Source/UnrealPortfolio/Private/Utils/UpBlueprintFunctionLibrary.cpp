// Copyright AB. All Rights Reserved.

#include "Utils/UpBlueprintFunctionLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
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

FText UUpBlueprintFunctionLibrary::GetInGameName(const UObject* WorldContextObject, const FGameplayTag& TagId)
{
	if (const auto GameMode = GetGameMode<AUnrealPortfolioGameModeBase>(WorldContextObject))
	{
		if (TagId.MatchesTag(TAG_Npc))
		{
			if (const auto NpcDataTable = GameMode->GetNpcDataTable())
			{
				TArray<FUpNpcData*> AllNpcDataRows;
				NpcDataTable->GetAllRows<FUpNpcData>(TEXT("NpcDataTable GetAllRows"), AllNpcDataRows);

				for (const auto NpcDataRow : AllNpcDataRows)
				{
					if (NpcDataRow->TagId.MatchesTagExact(TagId)) return NpcDataRow->Name;
				}
			}
		}
	}

	return FText::FromString(TagId.ToString());
}

FText UUpBlueprintFunctionLibrary::GetInGameNameifiedText(const UObject* WorldContextObject, const FText& InText)
{
	auto Result = FString(InText.ToString());
	
	if (const auto GameMode = GetGameMode<AUnrealPortfolioGameModeBase>(WorldContextObject))
	{
		if (const auto NpcDataTable = GameMode->GetNpcDataTable())
		{
			FRegexMatcher Matcher(FRegexPattern(TEXT("\\[.+?\\]")), InText.ToString());
			TArray<FUpNpcData*> AllNpcDataRows;
			uint8 i = 0;
	
			while (Matcher.FindNext())
			{
				// Just in case we have a bug somewhere, avoid an infinite loop.
				if (++i > 100) break;

				if (AllNpcDataRows.Num() <= 0)
				{
					NpcDataTable->GetAllRows(TEXT("NpcDataTable GetAllRows"), AllNpcDataRows);
				}
		
				auto CaptureGroup = Matcher.GetCaptureGroup(0);
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "[", "");
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "]", "");

				FUpNpcData NpcData;

				for (const auto NpcDataRow : AllNpcDataRows)
				{
					if (NpcDataRow->TagId.ToString() == CaptureGroup)
					{
						NpcData = *NpcDataRow;
						break;
					}
				}

				if (NpcData.IsValid())
				{
					UKismetStringLibrary::ReplaceInline(Result, "[" + CaptureGroup + "]", NpcData.Name.ToString());
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
	if (const auto TagIdable = Cast<IUpTagIdable>(Actor))
	{
		return TagIdable->GetTagId().MatchesTagExact(Tag);
	}

	return false;
}

bool UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition)
{
	if (EntityTagSpec.PlayerTagSpecs.Num() > 0)
	{
		const auto GameplayTagAsset = Cast<IGameplayTagAssetInterface>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));

		if (!GameplayTagAsset) return false;

		FGameplayTagContainer PlayerTags;
		GameplayTagAsset->GetOwnedGameplayTags(PlayerTags);
			
		for (const auto& PlayerTagSpec : EntityTagSpec.PlayerTagSpecs)
		{
			if (bProhibition && PlayerTags.HasTagExact(PlayerTagSpec.Tag)) return false;
			if (!bProhibition && !PlayerTags.HasTagExact(PlayerTagSpec.Tag)) return false;
		}
	}

	if (const auto GameMode = GetGameMode<AUnrealPortfolioGameModeBase>(WorldContextObject))
	{
		for (const auto& NpcTagToTagSpecMappings : EntityTagSpec.NpcTagSpecMappings)
		{
			FGameplayTagContainer NpcTags;
			GameMode->GetNpcCharacterTags(NpcTagToTagSpecMappings.Tag, NpcTags);
			
			for (const auto& NpcTagSpec : NpcTagToTagSpecMappings.TagSpecs)
			{
				if (bProhibition && NpcTags.HasTagExact(NpcTagSpec.Tag)) return false;
				if (!bProhibition && !NpcTags.HasTagExact(NpcTagSpec.Tag)) return false;
			}
		}
	}

	// TODO(P0): Finish implementation.

	return true;
}

void UUpBlueprintFunctionLibrary::ProcessEntityTagSpecGrants(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec)
{
	if (EntityTagSpec.PlayerTagSpecs.Num() > 0)
	{
		if (const auto Player = Cast<AUpPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0)))
		{
			for (const auto& PlayerTagSpec : EntityTagSpec.PlayerTagSpecs)
			{
				Player->GrantTagSpec(PlayerTagSpec);
			}
		}
	}

	if (const auto GameMode = GetGameMode<AUnrealPortfolioGameModeBase>(WorldContextObject))
	{
		for (const auto& NpcTagToTagSpecMappings : EntityTagSpec.NpcTagSpecMappings)
		{
			for (const auto& NpcTagSpec : NpcTagToTagSpecMappings.TagSpecs)
			{
				AUpNpcCharacter::GrantTagSpec(GameMode, NpcTagToTagSpecMappings.Tag, NpcTagSpec);
			}
		}
	}

	// TODO(P0): Finish implementation.
}

FVector UUpBlueprintFunctionLibrary::CalculateVelocity(const FVector& FromLocation, const FVector& ToLocation, const float Duration, const float GravityScale)
{
	return FVector(
	(ToLocation.X - FromLocation.X) / Duration,
	(ToLocation.Y - FromLocation.Y) / Duration,
	(ToLocation.Z - FromLocation.Z - FMath::Pow(Duration, 2) * 0.5f * GRAVITATIONAL_ACCELERATION * GravityScale) / Duration);
}
