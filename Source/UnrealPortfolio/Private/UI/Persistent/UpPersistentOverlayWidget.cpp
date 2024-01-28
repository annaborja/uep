// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentOverlayWidget.h"

#include "Characters/Player/UpPlayerController.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/UpHud.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UI/CharacterSwitcher/UpCharacterSwitcherWidget.h"
#include "UI/Dialogue/UpDialogueOverlayWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

bool UUpPersistentOverlayWidget::IsDescendentWidgetActivated() const
{
	if (IsMainMenuOpen()) return true;

	if (const auto WidgetContainer = GetCharacterSwitcherContainer(); WidgetContainer && WidgetContainer->GetActiveWidget())
	{
		return true;
	}
	
	if (const auto WidgetContainer = GetDialogueWidgetContainer(); WidgetContainer && WidgetContainer->GetActiveWidget())
	{
		return true;
	}

	return false;
}

bool UUpPersistentOverlayWidget::IsMainMenuOpen() const
{
	if (const auto WidgetContainer = GetMainMenuWidgetContainer())
	{
		return WidgetContainer->GetActiveWidget() != nullptr;
	}

	return false;
}

void UUpPersistentOverlayWidget::OpenMainMenu() const
{
	if (const auto MainMenuWidgetContainer = GetMainMenuWidgetContainer(); MainMenuWidgetContainer && CustomHud)
	{
		if (const auto WidgetClass = CustomHud->GetMenuSwitcherClass())
		{
			// `AddWidget()` will handle grabbing an existing instance of the given widget class (if any).
			MainMenuWidgetContainer->AddWidget<UUpCommonActivatableWidget>(WidgetClass, [this](UUpCommonActivatableWidget& AddedWidget)
			{
				AddedWidget.SetCustomHud(CustomHud);
			});
		}
	}
}

UUpCharacterSwitcherWidget* UUpPersistentOverlayWidget::OpenCharacterSwitcher() const
{
	UUpCharacterSwitcherWidget* Widget = nullptr;

	if (const auto CharacterSwitcherContainer = GetCharacterSwitcherContainer(); CharacterSwitcherContainer && CustomHud)
	{
		if (const auto WidgetClass = CustomHud->GetCharacterSwitcherClass())
		{
			// `AddWidget()` will handle grabbing an existing instance of the given widget class (if any).
			CharacterSwitcherContainer->AddWidget<UUpCharacterSwitcherWidget>(WidgetClass, [this, &Widget](UUpCharacterSwitcherWidget& AddedWidget)
			{
				Widget = &AddedWidget;
				AddedWidget.SetCustomHud(CustomHud);
			});
		}
	}
	
	return Widget;
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

ESlateVisibility UUpPersistentOverlayWidget::GetInteractionDisplayVisibility() const
{
	if (IsDescendentWidgetActivated()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentOverlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (const auto WidgetContainer = GetMainMenuWidgetContainer())
	{
		WidgetContainer->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedWidgetChanged);
	}

	if (const auto WidgetContainer = GetCharacterSwitcherContainer())
	{
		WidgetContainer->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedWidgetChanged);
	}

	if (const auto WidgetContainer = GetDialogueWidgetContainer())
	{
		WidgetContainer->OnDisplayedWidgetChanged().AddUObject(this, &ThisClass::OnDisplayedWidgetChanged);
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
