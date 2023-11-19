// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct FUpTagSpec
{
	GENERATED_BODY()

	FUpTagSpec() {}
	explicit FUpTagSpec(const FGameplayTag& InTag, const int32& InCount = 1) : Tag(InTag), Count(InCount) {}
	
	bool IsValid() const { return Tag.IsValid() && Count != 0; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RelatedTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;
};

USTRUCT()
struct FUpTagToTagSpecsMapping
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, meta=(TitleProperty="Tag"))
	TArray<FUpTagSpec> TagSpecs;
};

USTRUCT()
struct FUpEntityTagSpec
{
	GENERATED_BODY();

	FUpEntityTagSpec() {}
	explicit FUpEntityTagSpec(const TArray<FUpTagSpec>& InPlayerTagSpecs) : PlayerTagSpecs(InPlayerTagSpecs) {}
	
	UPROPERTY(EditAnywhere, meta=(TitleProperty="Tag"))
	TArray<FUpTagSpec> PlayerTagSpecs;
	
	UPROPERTY(EditAnywhere, meta=(TitleProperty="Tag"))
	TArray<FUpTagToTagSpecsMapping> NpcTagSpecMappings;
	UPROPERTY(EditAnywhere, meta=(TitleProperty="Tag"))
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
