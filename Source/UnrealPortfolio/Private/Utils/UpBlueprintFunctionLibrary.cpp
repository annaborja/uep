// Copyright AB. All Rights Reserved.

#include "Utils/UpBlueprintFunctionLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Tags/NpcTags.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/GeneralStructs.h"

// UE sometimes appends a numeric suffix (`_#`) to gameplay tags (possibly a UE bug?).
// This function normalizes these tags.
FGameplayTag UUpBlueprintFunctionLibrary::GetNormalizedTag(const FGameplayTag& Tag)
{
	TArray<FString> TagSegments;
	Tag.ToString().ParseIntoArray(TagSegments, TEXT("_"));

	return FGameplayTag::RequestGameplayTag(FName(TagSegments[0]));
}

bool UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition)
{
	if (!WorldContextObject) return false;

	const auto World = WorldContextObject->GetWorld();

	if (!World) return false;

	if (EntityTagSpec.PlayerTagSpecs.Num() > 0)
	{
		const auto PlayerController = World->GetFirstPlayerController();

		if (!PlayerController) return false;

		const auto GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(PlayerController->GetCharacter());

		if (!GameplayTagAssetInterface) return false;

		FGameplayTagContainer PlayerGameplayTags;
		GameplayTagAssetInterface->GetOwnedGameplayTags(PlayerGameplayTags);
			
		for (const auto& PlayerTagSpec : EntityTagSpec.PlayerTagSpecs)
		{
			if (bProhibition && PlayerGameplayTags.HasTagExact(PlayerTagSpec.Tag)) return false;
			if (!bProhibition && !PlayerGameplayTags.HasTagExact(PlayerTagSpec.Tag)) return false;
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

	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		for (const auto& NpcTagToTagSpecMappings : EntityTagSpec.NpcTagSpecMappings)
		{
			for (const auto& NpcTagSpec : NpcTagToTagSpecMappings.TagSpecs)
			{
				if (NpcTagToTagSpecMappings.Tag.MatchesTag(TAG_Npc))
				{
					AUpNpcCharacter::GrantTagSpec(GameMode, NpcTagToTagSpecMappings.Tag, NpcTagSpec);
				}
			}
		}
	}

	// TODO(P0): Finish implementation.
}

FText UUpBlueprintFunctionLibrary::GetInGameNameifiedText(const UObject* WorldContextObject, const FText& InText)
{
	auto Result = FString(InText.ToString());
	
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		if (const auto NpcDataTable = GameMode->GetNpcDataTable())
		{
			FRegexMatcher Matcher(FRegexPattern(TEXT("\\[.+?\\]")), InText.ToString());
			TArray<FUpNpcData*> AllRows;
			uint8 i = 0;
	
			while (Matcher.FindNext())
			{
				// Just in case we have a bug somewhere, avoid an infinite loop.
				if (++i > 100) break;

				if (AllRows.Num() <= 0)
				{
					NpcDataTable->GetAllRows(TEXT("NpcDataTable GetAllRows"), AllRows);
				}
		
				auto CaptureGroup = Matcher.GetCaptureGroup(0);
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "[", "");
				UKismetStringLibrary::ReplaceInline(CaptureGroup, "]", "");

				FUpNpcData NpcData;

				for (const auto Row : AllRows)
				{
					if (Row->TagId.ToString() == CaptureGroup)
					{
						NpcData = *Row;
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
