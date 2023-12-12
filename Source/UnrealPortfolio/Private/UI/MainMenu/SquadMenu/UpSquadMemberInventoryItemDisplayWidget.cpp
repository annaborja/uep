// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryItemDisplayWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpNpcCharacter.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberInventoryItemDisplayWidget::PopulateInventoryItemData(const FGameplayTag& ItemTagId, const FUpInventoryItemData& InventoryItemData, AUpNpcCharacter* Npc)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		ItemData = GameInstance->GetItemData(ItemTagId);

		if (Npc)
		{
			OnClicked().AddLambda([this, Npc]
			{
				// Npc->EquipItem(ItemData);
			});
		}
	}
}
