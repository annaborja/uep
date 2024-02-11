// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/JournalMenu/UpObjectiveDisplayWidget.h"

#include "CommonTextBlock.h"

FReply UUpObjectiveDisplayWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	SetStyle(FocusedButtonStyle);
	FocusDelegate.Broadcast();
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UUpObjectiveDisplayWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	SetStyle(ButtonStyle);
}

void UUpObjectiveDisplayWidget::SetCompleted(const bool bInCompleted)
{
	bCompleted = bInCompleted;

	if (const auto Widget = GetSummaryTextBlock())
	{
		Widget->SetStyle(bCompleted ? ObjectiveSummaryCompletedTextStyle : ObjectiveSummaryTextStyle);
	}
}
