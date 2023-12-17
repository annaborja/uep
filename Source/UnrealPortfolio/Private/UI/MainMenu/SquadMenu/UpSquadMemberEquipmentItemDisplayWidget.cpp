// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberEquipmentItemDisplayWidget.h"

#include "UpGameInstance.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberEquipmentItemDisplayWidget::PopulateEquipmentItemData(const FGameplayTag& ItemTagId)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		ItemData = GameInstance->GetItemData(ItemTagId);
	}
}
