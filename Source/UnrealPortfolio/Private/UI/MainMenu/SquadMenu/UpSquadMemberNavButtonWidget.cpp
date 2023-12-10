// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberNavButtonWidget.h"

void UUpSquadMemberNavButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsSelectable(true);
	SetIsToggleable(true);
	
	SetRenderOpacity(GetSelected() ? RenderOpacitySelected : RenderOpacityDeselected);
}

void UUpSquadMemberNavButtonWidget::NativeOnSelected(const bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	SetRenderOpacity(RenderOpacitySelected);
}

void UUpSquadMemberNavButtonWidget::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
	
	SetRenderOpacity(RenderOpacityDeselected);
}
