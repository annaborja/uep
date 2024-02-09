// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentNotificationDisplayWidget.h"

#include "UI/UpHud.h"

void UUpPersistentNotificationDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UUpPersistentNotificationDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->NotificationDelegate.AddUObject(this, &ThisClass::HandleNotification);
}

void UUpPersistentNotificationDisplayWidget::HandleNotification(const FUpNotificationData& NotificationData)
{
}

void UUpPersistentNotificationDisplayWidget::ClearData()
{
}
