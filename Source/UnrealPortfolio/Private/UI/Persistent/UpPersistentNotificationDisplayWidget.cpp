// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentNotificationDisplayWidget.h"

#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Animation/UMGSequencePlayer.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
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
	if (!NotificationQueue.IsValidIndex(0)) return;
	if (DisplayedNotifications.Num() >= MaxNotificationsDisplayed) return;

	const auto Notification = NotificationQueue[0];
	NotificationQueue.RemoveAt(0, 1, true);
	DisplayedNotifications.Add(Notification);
	UpdateDisplayedText();

	if (GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (const auto FadeAnimation = GetFadeAnimation())
		{
			PlayAnimationForward(FadeAnimation);
		}
	}
	
	if (const auto World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, Notification]
		{
			if (const auto Idx = DisplayedNotifications.IndexOfByPredicate([Notification](const FUpNotificationData& Data)
			{
				return Data.Text.EqualTo(Notification.Text);
			}); Idx != INDEX_NONE)
			{
				DisplayedNotifications.RemoveAt(Idx, 1, true);
				ProcessNotificationQueue();
			}
			
			if (DisplayedNotifications.Num() <= 0)
			{
				if (const auto FadeAnimation = GetFadeAnimation())
				{
					const auto SequencePlayer = PlayAnimationReverse(FadeAnimation);
					SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
					{
						SetVisibility(ESlateVisibility::Collapsed);
						UpdateDisplayedText();
					});
				}
			} else
			{
				UpdateDisplayedText();
			}
		}), Notification.TimeToLive, false);
	}
}

void UUpPersistentNotificationDisplayWidget::UpdateDisplayedText() const
{
	if (const auto Container = GetTextContainer())
	{
		Container->ClearChildren();
		uint8 WidgetIdx = 0;
		
		for (const auto Notification : DisplayedNotifications)
		{
			if (!WidgetTree) continue;

			const auto Widget = WidgetTree->ConstructWidget<UCommonRichTextBlock>(UCommonRichTextBlock::StaticClass());
			Widget->SetText(Notification.Text);
			
			if (NotificationTextStyle)
			{
				if (const auto TextStyle = Cast<UCommonTextStyle>(NotificationTextStyle.GetDefaultObject()))
				{
					FTextBlockStyle TextBlockStyle;
					TextStyle->ToTextBlockStyle(TextBlockStyle);
					
					Widget->SetDefaultTextStyle(TextBlockStyle);
					Widget->SetTextStyleSet(NotificationTextStyleSet);
				}
			}

			if (const auto PanelSlot = Cast<UVerticalBoxSlot>(Container->AddChild(Widget)); PanelSlot && WidgetIdx > 0)
			{
				PanelSlot->SetPadding(FMargin(0.f, NotificationGap, 0.f, 0.f));
			}

			WidgetIdx++;
		}
	}
}
