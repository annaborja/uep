// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/UpItem.h"
#include "UpAmmo.generated.h"

class AUpCharacter;

UCLASS()
class UNREALPORTFOLIO_API AUpAmmo : public AUpItem
{
	GENERATED_BODY()

public:
	static bool HandleAmmoTagSpecGrant(const AUpCharacter* Character, const FUpTagSpec& TagSpec);
	static bool TryGrantAmmoForWeaponSlot(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag& TargetTagId,
		const uint8 Quantity, const FUpEquipmentSlotData& EquipmentSlotData);
	
	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) override;

protected:
	virtual int32 GetInteractionQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const override;
	virtual FGameplayTag GetInteractionRelatedTag(const AUpPlayerController* PlayerController) const override;
};
