// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentEquipmentItemDisplayWidget.h"

FLinearColor UUpPersistentEquipmentItemDisplayWidget::GetImageColor() const
{
	if (bActivated) return ImageColor_Activated;

	return ImageColor_Deactivated;
}

ESlateVisibility UUpPersistentEquipmentItemDisplayWidget::GetCommonActionWidgetVisibility() const
{
	if (Image == Image_Empty) return ESlateVisibility::Collapsed;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentEquipmentItemDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Image = Image_Empty;
}
