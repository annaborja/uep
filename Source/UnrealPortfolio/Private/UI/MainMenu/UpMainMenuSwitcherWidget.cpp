// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/UpMainMenuSwitcherWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/UiTags.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/UpMainMenuTabListWidget.h"
#include "UI/MainMenu/UpMainMenuTabWidget.h"
#include "UI/MainMenu/InventoryMenu/UpInventoryMenuWidget.h"
#include "UI/MainMenu/JournalMenu/UpJournalMenuWidget.h"
#include "UI/MainMenu/QuestsMenu/UpQuestsMenuWidget.h"
#include "UI/MainMenu/ReputationMenu/UpReputationMenuWidget.h"
#include "UI/MainMenu/SettingsMenu/UpSettingsMenuWidget.h"
#include "UI/MainMenu/SkillsMenu/UpSkillsMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMenuWidget.h"
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
		if (const auto Menu = GetSquadMenu())
		{
			WidgetSwitcher->SetActiveWidget(Menu);
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

			if (const auto Menu = GetSquadMenu())
			{
				TabList->RegisterTab(TAG_Menu_Squad.GetTag().GetTagName(), MenuTabClass, Menu);
			}
			
			if (const auto Menu = GetQuestsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Quests.GetTag().GetTagName(), MenuTabClass, Menu);
			}

			if (const auto Menu = GetInventoryMenu())
			{
				TabList->RegisterTab(TAG_Menu_Inventory.GetTag().GetTagName(), MenuTabClass, Menu);
			}

			if (const auto Menu = GetStatsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Stats.GetTag().GetTagName(), MenuTabClass, Menu);
			}

			if (const auto Menu = GetSkillsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Skills.GetTag().GetTagName(), MenuTabClass, Menu);
			}
			
			if (const auto Menu = GetReputationMenu())
			{
				TabList->RegisterTab(TAG_Menu_Reputation.GetTag().GetTagName(), MenuTabClass, Menu);
			}

			if (const auto Menu = GetJournalMenu())
			{
				TabList->RegisterTab(TAG_Menu_Journal.GetTag().GetTagName(), MenuTabClass, Menu);
			}

			if (const auto Menu = GetSettingsMenu())
			{
				TabList->RegisterTab(TAG_Menu_Settings.GetTag().GetTagName(), MenuTabClass, Menu);
			}
		}
	}
}
