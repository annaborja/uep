// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

struct FUpTagSpec;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsInventoryTag(const FGameplayTag& Tag) { return Tag.MatchesTag(TAG_Item); }
	static bool IsWeaponTag(const FGameplayTag& Tag) { return Tag.MatchesTag(TAG_Item_Weapon); }
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec) { return IsInventoryTag(TagSpec.Tag); }
	
	UUpInventoryComponent();

	bool HandleTagSpecGrant(const FUpTagSpec& TagSpec) const;

private:
	bool HandleAmmoTagSpecGrant(const FUpTagSpec& TagSpec) const;
	bool TryGrantAmmoForWeaponSlot(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag& TargetTagId,
		const uint8 Quantity, const FUpEquipmentSlotData& EquipmentSlotData) const;
};
