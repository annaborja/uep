// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryItemDisplayWidget.h"

#include "UpGameInstance.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberInventoryItemDisplayWidget::PopulateInventoryItemData(const FUpInventoryItemData& InventoryItemData)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		ItemData = GameInstance->GetItemData(InventoryItemData.ItemTagId);
	}
}
