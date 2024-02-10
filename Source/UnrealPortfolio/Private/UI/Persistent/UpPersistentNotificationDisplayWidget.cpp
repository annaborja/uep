// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentNotificationDisplayWidget.h"

#include "Components/RichTextBlock.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

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

void UUpPersistentNotificationDisplayWidget::HandleNotification(const FUpNotificationData NotificationData)
{
	NotificationQueue.Add(NotificationData);
	ProcessNotificationQueue();
}

void UUpPersistentNotificationDisplayWidget::ProcessNotificationQueue()
{
	if (DisplayedNotifications.Num() >= MaxNumNotificationsDisplayed) return;
	if (!NotificationQueue.IsValidIndex(0)) return;

	const auto Notification = NotificationQueue[0];
	NotificationQueue.RemoveAt(0, 1, true);
	DisplayedNotifications.Add(Notification);
	UpdateTextWidget();
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	if (const auto World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, Notification]
		{
			if (const auto Idx = DisplayedNotifications.IndexOfByPredicate([Notification](const FUpNotificationData Data)
			{
				return Data.Text.EqualTo(Notification.Text);
			}); Idx != INDEX_NONE)
			{
				DisplayedNotifications.RemoveAt(Idx, 1, true);
				UpdateTextWidget();
			}
			
			if (DisplayedNotifications.Num() <= 0)
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
		}), Notification.TimeToLive, false);
	}
}

void UUpPersistentNotificationDisplayWidget::UpdateTextWidget() const
{
	if (const auto Widget = GetTextWidget())
	{
		FString WidgetText = TEXT("");

		for (const auto& Notification : DisplayedNotifications)
		{
			WidgetText += UUpBlueprintFunctionLibrary::GetInGameNameifiedText(this, Notification.Text).ToString();
		}
			
		Widget->SetText(FText::FromString(WidgetText));
	}
}
