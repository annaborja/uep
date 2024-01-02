// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentEquipmentDisplayWidget.h"

#include "UI/UpHud.h"
#include "UI/Persistent/UpPersistentEquipmentItemDisplayWidget.h"

void UUpPersistentEquipmentDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);
	
	if (!CustomHud) return;
	
	CustomHud->EquipmentUpdateDelegate.AddUObject(this, &ThisClass::HandleEquipmentUpdate);
	CustomHud->EquipmentActivationUpdateDelegate.AddUObject(this, &ThisClass::HandleEquipmentActivationUpdate);
}

void UUpPersistentEquipmentDisplayWidget::HandleEquipmentActivationUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const bool bActivated)
{
	for (const auto Widget : GetEquipmentItemDisplayWidgets())
	{
		if (Widget->GetEquipmentSlot() == EquipmentSlot)
		{
			Widget->SetActivated(bActivated);
			break;
		}
	}
}

void UUpPersistentEquipmentDisplayWidget::HandleEquipmentUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const FUpItemData& ItemData)
{
	for (const auto Widget : GetEquipmentItemDisplayWidgets())
	{
		if (Widget->GetEquipmentSlot() == EquipmentSlot)
		{
			if (ItemData.IsValid())
			{
				Widget->SetImage(ItemData.Image_Small);
			} else
			{
				Widget->ResetImage();
			}
			
			break;
		}
	}
}
