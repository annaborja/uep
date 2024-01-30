// Copyright AB. All Rights Reserved.

#include "Utils/UpBlueprintFunctionLibrary.h"

#include "GameplayEffectCustomApplicationRequirement.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/Components/UpPlayerPartyComponent.h"
#include "GAS/UpGasDataAsset.h"
#include "Items/UpItem.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Tags/ItemTags.h"
#include "Tags/NpcTags.h"
#include "Tags/ScriptTags.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Constants.h"
#include "Utils/Structs.h"

AUpPlayerController* UUpBlueprintFunctionLibrary::GetCustomPlayerController(const UObject* WorldContextObject)
{
	return Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

AUpPlayableCharacter* UUpBlueprintFunctionLibrary::GetPlayerPossessedCharacter(const UObject* WorldContextObject)
{
	if (const auto CustomPlayerController = GetCustomPlayerController(WorldContextObject))
	{
		return CustomPlayerController->GetPossessedCharacter();
	}

	return nullptr;
}

AUpHud* UUpBlueprintFunctionLibrary::GetCustomHud(const UObject* WorldContextObject)
{
	if (const auto CustomPlayerController = GetCustomPlayerController(WorldContextObject))
	{
		return CustomPlayerController->GetCustomHud();
	}

	return nullptr;
}

UUpGameInstance* UUpBlueprintFunctionLibrary::GetGameInstance(const UObject* WorldContextObject)
{
	return Cast<UUpGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

AUpLevelScriptActor* UUpBlueprintFunctionLibrary::GetLevelScriptActor(const UObject* WorldContextObject)
{
	if (const auto GameInstance = GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetLevelScriptActor();
	}

	return nullptr;
}

AUnrealPortfolioGameModeBase* UUpBlueprintFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
}

TArray<AActor*> UUpBlueprintFunctionLibrary::FindActors(const UObject* WorldContextObject, const TSubclassOf<AActor> ActorClass, const FGameplayTag& TagId)
{
	TArray<AActor*> Result;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, OutActors);

	for (const auto Actor : OutActors)
	{
		if (TagId.IsValid())
		{
			if (const auto TagIdable = Cast<IUpTagIdable>(Actor); TagIdable && TagIdable->GetTagId().MatchesTagExact(TagId))
			{
				Result.Add(Actor);
			}
		} else
		{
			Result.Add(Actor);
		}
	}

	return Result;
}

bool UUpBlueprintFunctionLibrary::SatisfiesActorParams(const FUpScriptActorParams& ActorParams, const AActor* Actor)
{
	if (ActorParams.ActorClass && !Actor->IsA(ActorParams.ActorClass)) return false;
	
	if (ActorParams.TagId.IsValid())
	{
		if (const auto TagIdable = Cast<IUpTagIdable>(Actor); !TagIdable || !TagIdable->GetTagId().MatchesTagExact(ActorParams.TagId))
		{
			return false;
		}
	}

	if (ActorParams.bPlayerOnly)
	{
		if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(Actor); !PlayableCharacter || !PlayableCharacter->IsPlayerControlled())
		{
			return false;
		}
	}

	if (ActorParams.bExcludePlayer)
	{
		if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(Actor); PlayableCharacter && PlayableCharacter->IsPlayerControlled())
		{
			return false;
		}
	}

	return true;
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

FName UUpBlueprintFunctionLibrary::GetBlackboardKeyFromTag(const FGameplayTag& Tag)
{
	if (Tag.MatchesTag(TAG_BlackboardKey))
	{
		TArray<FString> TagSegments;
		Tag.ToString().ParseIntoArray(TagSegments, TEXT("."));

		if (TagSegments.IsValidIndex(1)) return FName(TagSegments[1]);
	}

	UE_LOG(LogTemp, Error, TEXT("No blackboard key found for tag: %s"), *Tag.ToString())

	return NAME_None;
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
	if (Character->IsRelaxed()) return NAME_STRING_UNARMED;
	
	switch (Character->GetPosture())
	{
	case EUpCharacterPosture::ArmedPistol:
		return NAME_STRING_ARMED_PISTOL;
	case EUpCharacterPosture::ArmedRifle:
		return NAME_STRING_ARMED_RIFLE;
	default:
		return NAME_STRING_UNARMED;
	}
}

bool UUpBlueprintFunctionLibrary::CanGrantAmmo(const AUpWeapon* Weapon)
{
	if (const auto GameInstance = GetGameInstance(Weapon))
	{
		if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
		{
			if (const auto EffectClass = GasDataAsset->GetEffectClass_AmmoGrant())
			{
				if (const auto Effect = EffectClass->GetDefaultObject<UGameplayEffect>())
				{
					if (const auto AbilitySystemComponent = Weapon->GetAbilitySystemComponent())
					{
						for (const auto& AppReq : Effect->ApplicationRequirements)
						{
							if (*AppReq)
							{
								if (!AppReq->GetDefaultObject<UGameplayEffectCustomApplicationRequirement>()->
									CanApplyGameplayEffect(Effect, *AbilitySystemComponent->MakeOutgoingSpec(EffectClass,
										1.f, AbilitySystemComponent->MakeEffectContext()).Data.Get(), AbilitySystemComponent))
								{
									return false;
								}
							} else
							{
								return false;
							}
						}

						return true;
					}
				}
			}
		}
	}

	return false;
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
