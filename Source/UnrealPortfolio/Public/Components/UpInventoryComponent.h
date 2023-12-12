// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

class AStaticMeshActor;
struct FUpTagSpec;

UENUM(BlueprintType)
namespace EUpItemCategory
{
	enum Type : uint8
	{
		Consumable,
		Permanent,
		Weapon,
		Wearable
	};
}

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

UENUM()
namespace EUpCharacterEquipmentSlot
{
	enum Type : uint8
	{
		Weapon1,
		Weapon2
	};
}

USTRUCT(BlueprintType)
struct FUpItemData : public FTableRowBase
{
	GENERATED_BODY();

	bool IsValid() const { return TagId.IsValid() && !Name.IsEmpty(); }
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EUpItemCategory::Type> ItemCategory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AStaticMeshActor> StaticMeshActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Small;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Large;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EUpItemEquipmentSlot::Type> EquipmentSlot = EUpItemEquipmentSlot::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FUpTagSpec UsageEffect;
};

USTRUCT()
struct FUpInventoryItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TMap<uint8, uint8> ConditionQuantityMap;
};

USTRUCT()
struct FUpInventory
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FUpInventoryItemData> InventoryDataMap;
};

USTRUCT()
struct FUpItemInstance
{
	GENERATED_BODY()

	bool IsValid() const { return ItemTagId.IsValid() && Condition >= 0; }

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTagId;

	UPROPERTY(EditDefaultsOnly)
	int32 Condition = -1;
};

USTRUCT()
struct FUpCharacterEquipment
{
	GENERATED_BODY()

	void PopulateEquipmentSlot(const EUpCharacterEquipmentSlot::Type EquipmentSlot, const FUpItemInstance ItemInstance)
	{
		switch (EquipmentSlot)
		{
		case EUpCharacterEquipmentSlot::Weapon1:
			Weapon1 = ItemInstance;
			break;
		case EUpCharacterEquipmentSlot::Weapon2:
			Weapon2 = ItemInstance;
			break;
		default:
			break;
		}
	}

	UPROPERTY(EditDefaultsOnly)
	FUpItemInstance Weapon1;
	UPROPERTY(EditDefaultsOnly)
	FUpItemInstance Weapon2;
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
