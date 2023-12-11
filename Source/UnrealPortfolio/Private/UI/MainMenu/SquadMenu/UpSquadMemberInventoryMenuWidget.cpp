// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"

#include "UpGameInstance.h"
#include "Components/PanelWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryItemDisplayWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberInventoryMenuWidget::SetNpcTagId(const FGameplayTag& NpcTagId)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		Inventory = GameInstance->GetNpcInventory(NpcTagId);
	}

	PopulateInventoryItems();
}

void UUpSquadMemberInventoryMenuWidget::PopulateInventoryItems()
{
	if (const auto InventoryItemsContainer = GetInventoryItemsContainer(); InventoryItemsContainer && InventoryItemDisplayWidgetClass)
	{
		for (const auto InventoryData : Inventory.InventoryDataMap)
		{
			const auto Widget = CreateWidget<UUpSquadMemberInventoryItemDisplayWidget>(this, InventoryItemDisplayWidgetClass);
			Widget->SetCustomHud(CustomHud);
			Widget->PopulateInventoryItemData(InventoryData.Value);

			InventoryItemsContainer->AddChild(Widget);

			// if (const auto Slot = Cast<UVerticalBoxSlot>(PrimaryAttributesContainer->AddChild(Widget)); Slot && PrimaryAttributeIndex > 0)
			// {
				// Slot->SetPadding(FMargin(0.f, PrimaryAttributeRowGap, 0.f, 0.f));
			// }
		}
	}
}
