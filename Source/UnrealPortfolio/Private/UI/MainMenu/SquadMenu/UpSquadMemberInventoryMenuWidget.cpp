// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberEquipmentItemDisplayWidget.h"

void UUpSquadMemberInventoryMenuWidget::SetNpc(const AUpPlayableNpc* Npc) const
{
	const auto Equipment = Npc->GetCharacterEquipment();

	if (const auto Widget = GetWeapon1Display())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Weapon1);
	}

	if (const auto Widget = GetWeapon2Display())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Weapon2);
	}

	if (const auto Widget = GetItem1Display())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Item1);
	}

	if (const auto Widget = GetItem2Display())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Item2);
	}

	if (const auto Widget = GetHelmetDisplay())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Helmet);
	}

	if (const auto Widget = GetArmorDisplay())
	{
		PopulateEquipmentItemDisplay(Widget, Equipment, EUpEquipmentSlot::Armor);
	}
}

void UUpSquadMemberInventoryMenuWidget::PopulateEquipmentItemDisplay(UUpSquadMemberEquipmentItemDisplayWidget* Widget,
	const FUpCharacterEquipment& Equipment, const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto EquipmentSlotDataPtr = Equipment.EquipmentSlotMap.Find(EquipmentSlot))
	{
		if (const auto EquipmentSlotData = *EquipmentSlotDataPtr; EquipmentSlotData.IsValid())
		{
			Widget->PopulateEquipmentItemData(EquipmentSlotData.ItemInstance.ItemTagId);
		}
	}
}
