// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Structs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpBlueprintFunctionLibrary.generated.h"

class AUnrealPortfolioGameModeBase;
class AUpHud;
struct FGameplayTag;
struct FUpEntityTagSpec;
struct FUpTagSpec;
class UUpGameInstance;

UCLASS()
class UNREALPORTFOLIO_API UUpBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AUpHud* GetCustomHud(const UObject* WorldContextObject);
	static UUpGameInstance* GetGameInstance(const UObject* WorldContextObject);
	static AUnrealPortfolioGameModeBase* GetGameMode(const UObject* WorldContextObject);

	static FText GetInGameName(const UObject* WorldContextObject, const FGameplayTag& TagId);
	static FText GetInGameNameifiedText(const UObject* WorldContextObject, const FText InText);
	
	static FGameplayTag GetNormalizedTag(const FGameplayTag& Tag);
	static bool HasTag(const AActor* Actor, const FGameplayTag& Tag);
	static bool HasTagId(const AActor* Actor, const FGameplayTag& Tag);
	
	static bool ValidateTag(const FGameplayTag& Tag, const FString FuncName);
	static bool ValidateItemTag(const FGameplayTag& Tag, const FString FuncName);
	static bool ValidateNpcTag(const FGameplayTag& Tag, const FString FuncName);
	static bool ValidateWeaponTag(const FGameplayTag& Tag, const FString FuncName);

	static bool IsTwoHandedGunTag(const FGameplayTag& Tag);
	
	static bool IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition = false);
	static void ProcessEntityTagSpecGrants(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec);
	
	static bool ValidateTagSpec(const FUpTagSpec& TagSpec, const FString FuncName);
	
	static void GrantNpcTagSpec(const UObject* WorldContextObject, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec);
	static void GrantPlayerTagSpec(const UObject* WorldContextObject, const FUpTagSpec& TagSpec);

	static FVector CalculateVelocity(const FVector& FromLocation, const FVector& ToLocation, const float Duration, const float GravityScale = 1.f);
};
