// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/UpCharacter.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Tags/ItemTags.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

class AStaticMeshActor;
struct FUpTagSpec;
class UGameplayAbility;
class UUpGameplayAbility;

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
namespace EUpEquipmentSlot
{
	enum Type : uint8
	{
		Weapon1,
		Weapon2,
		Item1,
		Item2,
		Helmet,
		Armor
	};
}

UENUM()
namespace EUpAbilityGrantDuration
{
	enum Type : uint8
	{
		WhileEquipped,
		Permanent
	};
}

USTRUCT(BlueprintType)
struct FUpAbilityGrantSpec
{
	GENERATED_BODY()

	bool IsValid() const { return AbilityClass != nullptr; }

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpAbilityGrantDuration::Type> GrantDuration = EUpAbilityGrantDuration::Permanent;
};

USTRUCT(BlueprintType)
struct FUpItemData : public FTableRowBase
{
	GENERATED_BODY();

	bool IsValid() const { return TagId.IsValid() && !Name.IsEmpty(); }
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EUpItemCategory::Type> ItemCategory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Small;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Large;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStaticMeshActor> StaticMeshActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpCharacterPosture::Type> ResultingPosture = EUpCharacterPosture::Casual;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag = TAG_Socket_None;
	
	UPROPERTY(EditDefaultsOnly)
	FUpTagSpec UsageEffect;
	UPROPERTY(EditDefaultsOnly)
	TArray<FUpAbilityGrantSpec> AbilityGrantSpecs;
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
struct FUpEquipmentSlotData
{
	GENERATED_BODY()

	FUpEquipmentSlotData() {}
	explicit FUpEquipmentSlotData(const FUpItemInstance InItemInstance, const bool bInActivated = false) :
		ItemInstance(InItemInstance), bActivated(bInActivated) {}

	bool IsValid() const { return ItemInstance.IsValid(); }

	UPROPERTY(EditDefaultsOnly)
	FUpItemInstance ItemInstance;
	
	UPROPERTY(EditDefaultsOnly)
	bool bActivated = false;
};

USTRUCT()
struct FUpCharacterEquipment
{
	GENERATED_BODY()

	void ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.FindOrAdd(EquipmentSlot).bActivated = true;
	}
	
	void DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.FindOrAdd(EquipmentSlot).bActivated = false;
	}

	FUpEquipmentSlotData GetEquipmentSlotData(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		return EquipmentSlotMap.FindOrAdd(EquipmentSlot);
	}

	void PopulateEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot, const FUpItemInstance& ItemInstance)
	{
		EquipmentSlotMap.Add(EquipmentSlot, FUpEquipmentSlotData(ItemInstance));
	}

	void EmptyEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.Add(EquipmentSlot, FUpEquipmentSlotData());
	}

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EUpEquipmentSlot::Type>, FUpEquipmentSlotData> EquipmentSlotMap;
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
