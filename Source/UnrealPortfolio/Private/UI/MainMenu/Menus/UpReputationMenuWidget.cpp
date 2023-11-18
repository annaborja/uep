// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/Menus/UpReputationMenuWidget.h"

#include "UI/UpHud.h"

void UUpReputationMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (CustomHud)
	{
		PlayerKarma = CustomHud->GetPlayerKarma();
		PlayerRelationshipData = CustomHud->GetPlayerRelationshipData();
	}
}
