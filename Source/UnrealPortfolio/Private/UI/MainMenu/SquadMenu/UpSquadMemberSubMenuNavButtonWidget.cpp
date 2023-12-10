// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberSubMenuNavButtonWidget.h"

void UUpSquadMemberSubMenuNavButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsSelectable(true);
	SetIsToggleable(true);
}
