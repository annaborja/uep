// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/JournalMenu/UpObjectiveDisplayWidget.h"

#include "CommonTextBlock.h"

void UUpObjectiveDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetIsFocusable(true);
}

FReply UUpObjectiveDisplayWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FocusDelegate.Broadcast();
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UUpObjectiveDisplayWidget::SetCompleted(const bool bInCompleted)
{
	bCompleted = bInCompleted;

	if (const auto Widget = GetSummaryTextBlock())
	{
		Widget->SetStyle(bCompleted ? ObjectiveSummaryCompletedTextStyle : ObjectiveSummaryTextStyle);
	}
}
