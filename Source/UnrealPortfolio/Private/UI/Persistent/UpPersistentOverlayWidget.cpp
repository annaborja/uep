// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentOverlayWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "UI/UpHud.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UI/Dialogue/UpDialogueOverlayWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UUpPersistentOverlayWidget::OpenMainMenu() const
{
	if (const auto MenuWidgetContainer = GetMenuWidgetContainer(); MenuWidgetContainer && CustomHud)
	{
		if (const auto WidgetClass = CustomHud->GetMenuSwitcherClass())
		{
			// `AddWidget()` will handle grabbing an existing instance of the given widget class (if any).
			MenuWidgetContainer->AddWidget<UUpCommonActivatableWidget>(WidgetClass, [this](UUpCommonActivatableWidget& AddedWidget)
			{
				AddedWidget.SetCustomHud(CustomHud);
			});
		}
	}
}

UUpDialogueOverlayWidget* UUpPersistentOverlayWidget::OpenDialogueFlow() const
{
	UUpDialogueOverlayWidget* Widget = nullptr;

	if (const auto DialogueWidgetContainer = GetDialogueWidgetContainer(); DialogueWidgetContainer && CustomHud)
	{
		if (const auto WidgetClass = CustomHud->GetDialogueOverlayClass())
		{
			// `AddWidget()` will handle grabbing an existing instance of the given widget class (if any).
			DialogueWidgetContainer->AddWidget<UUpDialogueOverlayWidget>(WidgetClass, [this, &Widget](UUpDialogueOverlayWidget& AddedWidget)
			{
				Widget = &AddedWidget;
				AddedWidget.SetCustomHud(CustomHud);
			});
		}
	}
	
	return Widget;
}

ESlateVisibility UUpPersistentOverlayWidget::GetTargetInteractableDisplayVisibility() const
{
	if (IsDescendentWidgetActivated()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentOverlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (const auto DialogueWidgetContainer = GetDialogueWidgetContainer())
	{
		DialogueWidgetContainer->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedWidgetChanged);
	}

	if (const auto MenuWidgetContainer = GetMenuWidgetContainer())
	{
		MenuWidgetContainer->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedWidgetChanged);
	}
}

void UUpPersistentOverlayWidget::OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget) const
{
	// If the final activatable widget has been deactivated, reset the UI input config
	// (otherwise, Common UI will use the input config of the most recent active widget).
	if (!Widget)
	{
		if (const auto ActionRouter = UCommonUIActionRouterBase::Get(*this))
		{
			ActionRouter->SetActiveUIInputConfig(FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently));
		}
	}
}

bool UUpPersistentOverlayWidget::IsDescendentWidgetActivated() const
{
	if (const auto MenuWidgetContainer = GetMenuWidgetContainer(); MenuWidgetContainer && MenuWidgetContainer->GetActiveWidget())
	{
		return true;
	}
	
	if (const auto DialogueWidgetContainer = GetDialogueWidgetContainer(); DialogueWidgetContainer && DialogueWidgetContainer->GetActiveWidget())
	{
		return true;
	}

	return false;
}