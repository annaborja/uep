// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/UpMainMenuSwitcherWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/UiTags.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/UpMainMenuTabListWidget.h"
#include "UI/MainMenu/UpMainMenuTabWidget.h"
#include "UI/MainMenu/ReputationMenu/UpReputationMenuWidget.h"
#include "UI/MainMenu/StatsMenu/UpStatsMenuWidget.h"

void UUpMainMenuSwitcherWidget::HandleCloseMenuAction()
{
	DeactivateWidget();
}

void UUpMainMenuSwitcherWidget::HandleQuitGameAction() const
{
	if (!CustomHud) return;
	
	UKismetSystemLibrary::QuitGame(this, CustomHud->GetOwningPlayerController(),
		EQuitPreference::Quit, false);
}

void UUpMainMenuSwitcherWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bDelegatesAdded)
	{
		OnActivated().AddUObject(this, &ThisClass::SetUpTabList);
		OnDeactivated().AddUObject(this, &ThisClass::ResumeGame);
		bDelegatesAdded = true;
	}
}

void UUpMainMenuSwitcherWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (const auto WidgetSwitcher = GetWidgetSwitcher())
	{
		if (const auto StatsMenu = GetStatsMenu())
		{
			WidgetSwitcher->SetActiveWidget(StatsMenu);
		}
	}
}

void UUpMainMenuSwitcherWidget::SetUpTabList()
{
	if (const auto WidgetSwitcher = GetWidgetSwitcher())
	{
		if (const auto TabList = GetTabList(); TabList && TabList->GetTabCount() <= 0 && MenuTabClass)
		{
			TabList->SetLinkedSwitcher(WidgetSwitcher);

			if (const auto StatsMenu = GetStatsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Stats.GetTag().GetTagName(), MenuTabClass, StatsMenu);
			}

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
