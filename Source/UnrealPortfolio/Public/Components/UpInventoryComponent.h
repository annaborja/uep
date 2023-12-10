// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

struct FUpTagSpec;

UENUM()
namespace EUpItemEquipmentSlot
{
	enum Type : uint8
	{
		None,
		DominantHand,
		NonDominantHand,
		EitherHand,
		BothHands
	};
}

USTRUCT()
struct FUpItemData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemCategory;
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Image;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpItemEquipmentSlot::Type> EquipmentSlot = EUpItemEquipmentSlot::None;
	UPROPERTY(EditDefaultsOnly)
	FUpTagSpec UsageEffect;
};

USTRUCT()
struct FUpInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTagId;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 Condition = 100;
	UPROPERTY(EditDefaultsOnly)
	uint8 Quantity = 1;
};

USTRUCT()
struct FUpInventory
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<uint8, FUpInventoryItemData> ItemConditionMap;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsInventoryTag(const FGameplayTag& Tag);
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec);
	static bool HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec);
	
	UUpInventoryComponent();
};
