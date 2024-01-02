// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentEquipmentItemDisplayWidget.h"

FLinearColor UUpPersistentEquipmentItemDisplayWidget::GetImageColor() const
{
	if (bActivated) return ImageColor_Activated;

	return ImageColor_Deactivated;
}

void UUpPersistentEquipmentItemDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Image = Image_Empty;
}
