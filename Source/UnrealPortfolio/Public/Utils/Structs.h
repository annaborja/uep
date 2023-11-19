// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Structs.generated.h"

USTRUCT()
struct FUpTagSpec
{
	GENERATED_BODY()

	FUpTagSpec() {}
	explicit FUpTagSpec(const FGameplayTag& InTag, const int32& InCount = 1) : Tag(InTag), Count(InCount) {}
	
	bool IsValid() const { return Tag.IsValid() && Count != 0; }
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag RelatedTag;
	
	UPROPERTY(EditAnywhere)
	int32 Count = 1;
};

USTRUCT()
struct FUpTagToTagSpecsMapping
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere)
	TArray<FUpTagSpec> TagSpecs;
};

USTRUCT()
struct FUpEntityTagSpec
{
	GENERATED_BODY();

	FUpEntityTagSpec() {}
	explicit FUpEntityTagSpec(const TArray<FUpTagSpec>& InPlayerTagSpecs) : PlayerTagSpecs(InPlayerTagSpecs) {}
	
	UPROPERTY(EditAnywhere)
	TArray<FUpTagSpec> PlayerTagSpecs;
	
	UPROPERTY(EditAnywhere)
	TArray<FUpTagToTagSpecsMapping> NpcTagSpecMappings;
	UPROPERTY(EditAnywhere)
	TArray<FUpTagToTagSpecsMapping> WorldTagSpecMappings;
};

USTRUCT()
struct FUpMontageData
{
	GENERATED_BODY()

	bool IsValid() const { return Montage != nullptr; }
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditDefaultsOnly)
	float Rate = 1.f;
	UPROPERTY(EditDefaultsOnly)
	FName StartSection;
};
