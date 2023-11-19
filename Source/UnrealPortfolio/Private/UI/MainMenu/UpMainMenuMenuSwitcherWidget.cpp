// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/UpMainMenuMenuSwitcherWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/UiTags.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/UpMainMenuTabListWidget.h"
#include "UI/MainMenu/UpMainMenuTabWidget.h"
#include "UI/MainMenu/ReputationMenu/UpReputationMenuWidget.h"

void UUpMainMenuMenuSwitcherWidget::HandleCloseMenuAction()
{
	DeactivateWidget();
}

void UUpMainMenuMenuSwitcherWidget::HandleQuitGameAction() const
{
	if (!CustomHud) return;
	
	UKismetSystemLibrary::QuitGame(this, CustomHud->GetOwningPlayerController(),
		EQuitPreference::Quit, false);
}

void UUpMainMenuMenuSwitcherWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bDelegatesAdded)
	{
		OnActivated().AddUObject(this, &ThisClass::SetUpTabList);
		OnDeactivated().AddUObject(this, &ThisClass::ResumeGame);
		bDelegatesAdded = true;
	}
}

void UUpMainMenuMenuSwitcherWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(this, false);
}

void UUpMainMenuMenuSwitcherWidget::SetUpTabList()
{
	if (const auto WidgetSwitcher = GetWidgetSwitcher())
	{
		if (const auto TabList = GetTabList(); TabList && TabList->GetTabCount() <= 0 && MenuTabClass)
		{
			TabList->SetLinkedSwitcher(WidgetSwitcher);

			if (const auto ReputationMenu = GetReputationMenu())
			{
				TabList->RegisterTab(TAG_Menu_Reputation.GetTag().GetTagName(), MenuTabClass, ReputationMenu);
			}

			if (const auto SettingsMenu = GetSettingsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Settings.GetTag().GetTagName(), MenuTabClass, SettingsMenu);
			}
		}
	}
}
