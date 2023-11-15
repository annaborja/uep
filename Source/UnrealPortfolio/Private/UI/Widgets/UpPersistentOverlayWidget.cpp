// Copyright AB. All Rights Reserved.

#include "UI/Widgets/UpPersistentOverlayWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "UI/UpHud.h"
#include "UI/Widgets/UpCommonActivatableWidget.h"
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

void UUpPersistentOverlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

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
