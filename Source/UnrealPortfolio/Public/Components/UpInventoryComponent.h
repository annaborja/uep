// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Tags/ItemTags.h"
#include "Utils/Enums.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

class AUpWeapon;
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
	TEnumAsByte<EUpItemCategory::Type> ItemCategory = EUpItemCategory::Weapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Small;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Large;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AUpWeapon> StaticMeshActorClass;
	
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

USTRUCT(BlueprintType)
struct FUpItemInstance
{
	GENERATED_BODY()

	bool IsValid() const { return ItemTagId.IsValid() && Condition >= 0; }

	UPROPERTY(EditAnywhere)
	FGameplayTag ItemTagId;

	UPROPERTY(EditAnywhere)
	int32 Condition = -1;
};

USTRUCT(BlueprintType)
struct FUpEquipmentSlotData
{
	GENERATED_BODY()

	FUpEquipmentSlotData() {}
	explicit FUpEquipmentSlotData(const FUpItemInstance InItemInstance, const bool bInActivated = false) :
		ItemInstance(InItemInstance), bActivated(bInActivated) {}

	bool IsValid() const { return ItemInstance.IsValid(); }

	UPROPERTY(EditAnywhere)
	FUpItemInstance ItemInstance;
	
	UPROPERTY(EditAnywhere)
	bool bActivated = false;
};

USTRUCT(BlueprintType)
struct FUpCharacterEquipment
{
	GENERATED_BODY()

	void ActivateEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.FindOrAdd(EquipmentSlot).bActivated = true;
	}
	
	void DeactivateEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot)
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

	UPROPERTY(EditAnywhere)
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
