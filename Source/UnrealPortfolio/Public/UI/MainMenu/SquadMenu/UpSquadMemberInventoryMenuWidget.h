// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "Utils/Enums.h"
#include "UpSquadMemberInventoryMenuWidget.generated.h"

class AUpPlayableNpc;
struct FUpCharacterEquipment;
class UUpSquadMemberEquipmentItemDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberInventoryMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetNpc(const AUpPlayableNpc* Npc) const;

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
	static void PopulateEquipmentItemDisplay(UUpSquadMemberEquipmentItemDisplayWidget* Widget, const FUpCharacterEquipment& Equipment, const EUpEquipmentSlot::Type EquipmentSlot);
};
