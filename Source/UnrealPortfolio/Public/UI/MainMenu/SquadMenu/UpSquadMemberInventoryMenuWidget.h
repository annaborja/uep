// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpInventoryComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpSquadMemberInventoryMenuWidget.generated.h"

class AUpPlayableNpc;
struct FGameplayTag;
struct FUpCharacterEquipment;
class UUpSquadMemberEquipmentItemDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberInventoryMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetNpc(AUpPlayableNpc* InNpc);
	void SetNpcTagId(const FGameplayTag& NpcTagId) const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetWeapon1Display() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetWeapon2Display() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetItem1Display() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetItem2Display() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetHelmetDisplay() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetArmorDisplay() const;
	
private:
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableNpc> Npc;

	void PopulateEquipment(const FGameplayTag& NpcTagId) const;
	static void PopulateEquipmentItemDisplay(UUpSquadMemberEquipmentItemDisplayWidget* Widget, const FUpCharacterEquipment& Equipment, const EUpEquipmentSlot::Type EquipmentSlot);
};
