// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpBlueprintFunctionLibrary.generated.h"

struct FGameplayTag;
struct FUpEntityTagSpec;

UCLASS()
class UNREALPORTFOLIO_API UUpBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayTag GetNormalizedTag(const FGameplayTag& Tag);
	static bool IsEntityTagSpecSatisfied(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec, const bool bProhibition = false);
	static void ProcessEntityTagSpecGrants(const UObject* WorldContextObject, const FUpEntityTagSpec& EntityTagSpec);

	static FText GetInGameNameifiedText(const UObject* WorldContextObject, const FText& InText);
};
