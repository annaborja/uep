// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpGasDataAsset.generated.h"

class UGameplayAbility;

UCLASS()
class UNREALPORTFOLIO_API UUpGasDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetCommonStartupAbilityClasses() const { return CommonStartupAbilityClasses; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> CommonStartupAbilityClasses;
};
