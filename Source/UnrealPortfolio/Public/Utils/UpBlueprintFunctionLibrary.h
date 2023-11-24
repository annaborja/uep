// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UpBlueprintFunctionLibrary.generated.h"

class AUpHud;
struct FGameplayTag;
struct FUpEntityTagSpec;

UCLASS()
class UNREALPORTFOLIO_API UUpBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename T>
	static T* GetGameMode(const UObject* WorldContextObject);

	static FText GetInGameName(const UObject* WorldContextObject, const FGameplayTag& TagId);
	static FText GetInGameNameifiedText(const UObject* WorldContextObject, const FText& InText);
	
	static FGameplayTag GetNormalizedTag(const FGameplayTag& Tag);
	static bool HasTag(const AActor* Actor, const FGameplayTag& Tag);
	static bool HasTagId(const AActor* Actor, const FGameplayTag& Tag);
	static bool IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition = false);
	static void ProcessEntityTagSpecGrants(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec);

	static FVector CalculateVelocity(const FVector& FromLocation, const FVector& ToLocation, const float Duration, const float GravityScale = 1.f);
};

template <typename T>
T* UUpBlueprintFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return Cast<T>(UGameplayStatics::GetGameMode(WorldContextObject));
}
