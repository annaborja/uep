// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Tags/ItemTags.h"
#include "Structs.generated.h"

class UNiagaraSystem;
class AUpItem;

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
struct FUpInteractionData
{
	GENERATED_BODY()

	FUpInteractionData() {}
	explicit FUpInteractionData(AActor* InInteractable, const FText InInteractionPromptText, const FText InInteractionPromptSubText = FText::GetEmpty()) :
		Interactable(InInteractable), InteractionPromptText(InInteractionPromptText), InteractionPromptSubText(InInteractionPromptSubText) {}

	bool operator==(const FUpInteractionData& Other) const {
		return Interactable == Other.Interactable && InteractionPromptText.EqualTo(Other.InteractionPromptText) &&
			InteractionPromptSubText.EqualTo(Other.InteractionPromptSubText);
	}
	
	bool operator!=(const FUpInteractionData& Other) const {
		return !(*this == Other);
	}

	UPROPERTY()
	TObjectPtr<AActor> Interactable;

	UPROPERTY(BlueprintReadOnly)
	FText InteractionPromptText;
	UPROPERTY(BlueprintReadOnly)
	FText InteractionPromptSubText;
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AUpItem> ItemClass;

	UPROPERTY(Transient, VisibleInstanceOnly)
	TObjectPtr<AUpItem> ItemActor;
};

USTRUCT(BlueprintType)
struct FUpEquipmentSlotData
{
	GENERATED_BODY()

	FUpEquipmentSlotData() {}
	explicit FUpEquipmentSlotData(const FUpItemInstance InItemInstance, const bool bInActivated = false) :
		ItemInstance(InItemInstance), bActivated(bInActivated) {}

	UPROPERTY(EditAnywhere)
	FUpItemInstance ItemInstance;
	
	UPROPERTY(EditAnywhere)
	bool bActivated = false;
};

USTRUCT(BlueprintType)
struct FUpCharacterEquipment
{
	GENERATED_BODY()

	static TArray<EUpEquipmentSlot::Type> GetWeaponSlots()
	{
		return TArray { EUpEquipmentSlot::Weapon1, EUpEquipmentSlot::Weapon2 };
	}

	static bool IsWeaponSlot(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		for (const auto WeaponSlot : GetWeaponSlots())
		{
			if (EquipmentSlot == WeaponSlot) return true;
		}

		return false;
	}

	FUpCharacterEquipment()
	{
		EquipmentSlotMap.Add(EUpEquipmentSlot::Weapon1, FUpEquipmentSlotData());
		EquipmentSlotMap.Add(EUpEquipmentSlot::Weapon2, FUpEquipmentSlotData());
		EquipmentSlotMap.Add(EUpEquipmentSlot::Armor, FUpEquipmentSlotData());
		EquipmentSlotMap.Add(EUpEquipmentSlot::Helmet, FUpEquipmentSlotData());
		EquipmentSlotMap.Add(EUpEquipmentSlot::Item1, FUpEquipmentSlotData());
		EquipmentSlotMap.Add(EUpEquipmentSlot::Item2, FUpEquipmentSlotData());
	}
	
	FUpEquipmentSlotData GetEquipmentSlotData(const EUpEquipmentSlot::Type EquipmentSlot) const
	{
		return EquipmentSlotMap.FindChecked(EquipmentSlot);
	}

	void ActivateEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.FindChecked(EquipmentSlot).bActivated = true;
	}
	
	void DeactivateEquipmentSlot(const EUpEquipmentSlot::Type EquipmentSlot)
	{
		EquipmentSlotMap.FindChecked(EquipmentSlot).bActivated = false;
	}

	EUpEquipmentSlot::Type GetPotentialActiveWeaponSlot() const
	{
		for (const auto EquipmentSlot : GetWeaponSlots())
		{
			if (const auto& EquipmentSlotData = GetEquipmentSlotData(EquipmentSlot); EquipmentSlotData.bActivated)
			{
				return EquipmentSlot;
			}
		}

		return EUpEquipmentSlot::Weapon1;
	}

	FUpEquipmentSlotData GetPotentialActiveWeaponSlotData() const
	{
		return GetEquipmentSlotData(GetPotentialActiveWeaponSlot());
	}
	
	void SetEquipmentSlotActor(const EUpEquipmentSlot::Type EquipmentSlot, AUpItem* InItemActor)
	{
		EquipmentSlotMap.FindChecked(EquipmentSlot).ItemInstance.ItemActor = InItemActor;
	}
	
	void SetEquipmentSlotClass(const EUpEquipmentSlot::Type EquipmentSlot, const TSubclassOf<AUpItem> InItemClass)
	{
		EquipmentSlotMap.FindChecked(EquipmentSlot).ItemInstance.ItemClass = InItemClass;
	}

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EUpEquipmentSlot::Type>, FUpEquipmentSlotData> EquipmentSlotMap;
};

USTRUCT()
struct FUpMontageData
{
	GENERATED_BODY()
	
	FUpMontageData() {}
	explicit FUpMontageData(UAnimMontage* InMontage, const float InRate = 1.f, const FName& InStartSection = FName()) :
		Montage(InMontage), Rate(InRate), StartSection(InStartSection) {}

	bool IsValid() const { return Montage != nullptr; }
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditDefaultsOnly)
	float Rate = 1.f;
	UPROPERTY(EditDefaultsOnly)
	FName StartSection;
};

USTRUCT(BlueprintType)
struct FUpTagSpec
{
	GENERATED_BODY()

	FUpTagSpec() {}
	explicit FUpTagSpec(const FGameplayTag& InTag, const int32& InCount = 1) : Tag(InTag), Count(InCount) {}
	
	bool IsValid() const { return Tag.IsValid(); }

	FString ToString() const
	{
		if (RelatedTag.IsValid()) return FString::Printf(TEXT("TagSpec %s / %s (%d)"), *Tag.ToString(), *RelatedTag.ToString(), Count);
		
		return FString::Printf(TEXT("TagSpec %s (%d)"), *Tag.ToString(), Count);
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RelatedTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RelatedCount = 0.f;
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
	TEnumAsByte<EUpCharacterPosture::Type> ResultingPosture = EUpCharacterPosture::Casual;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag = TAG_Socket_None;
	
	UPROPERTY(EditDefaultsOnly)
	FUpTagSpec UsageEffect;
	UPROPERTY(EditDefaultsOnly)
	TArray<FUpAbilityGrantSpec> AbilityGrantSpecs;
};

USTRUCT(BlueprintType)
struct FUpTutorialData : public FTableRowBase
{
	GENERATED_BODY()

	bool IsValid() const { return TagId.IsValid() && !Text.IsEmpty(); }

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Text;

	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="RowName"))
	TArray<FDataTableRowHandle> InputActionRows;
	
	UPROPERTY(EditDefaultsOnly)
	float TimeToLive = 5.f;
};

USTRUCT()
struct FUpWeaponData : public FTableRowBase
{
	GENERATED_BODY();

	bool IsValid() const { return TagId.IsValid() && TagId.MatchesTag(TAG_Item_Weapon); }
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 MagazineCapacity = 0;
	UPROPERTY(EditDefaultsOnly)
	uint8 MaxAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpWeaponFiringMode::Type> FiringMode = EUpWeaponFiringMode::SemiAutomatic;
	UPROPERTY(EditDefaultsOnly)
	uint8 BurstSize = 1;
	UPROPERTY(EditDefaultsOnly)
	float BurstShotInterval = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	float BurstInterval = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	float Range = 20000.f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> NiagaraSystem_Impact;
};
