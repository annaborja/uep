// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"

void UUpSquadMemberInventoryMenuWidget::SetAbilitySystemComponent(UUpAbilitySystemComponent* InAbilitySystemComponent)
{
}

void UUpSquadMemberInventoryMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulatePrimaryAttributes();
}

void UUpSquadMemberInventoryMenuWidget::PopulatePrimaryAttributes()
{
}
