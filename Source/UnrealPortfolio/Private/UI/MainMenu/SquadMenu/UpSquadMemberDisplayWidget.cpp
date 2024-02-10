// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UpGameInstance.h"
#include "Tags/UiTags.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberUpgradesMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberStatsMenuWidget.h"

void UUpSquadMemberDisplayWidget::SetNpc(AUpPlayableNpc* InNpc)
{
	Npc = InNpc;
	CharacterData = Npc->GetCharacterData();
	
	PopulateSubMenuData();
}

void UUpSquadMemberDisplayWidget::SetCharacterData(const FUpCharacterData InCharacterData)
{
	Npc = nullptr;
	CharacterData = InCharacterData;
	
	PopulateSubMenuData();
}

void UUpSquadMemberDisplayWidget::SetActiveSubMenu(const FGameplayTag& SubMenuTag) const
{
	if (const auto SubMenuSwitcher = GetSubMenuSwitcher())
	{
		UWidget* Widget;

		if (SubMenuTag.MatchesTagExact(TAG_Menu_SquadMember_Inventory))
		{
			Widget = GetSquadMemberInventoryMenu();
		} else if (SubMenuTag.MatchesTagExact(TAG_Menu_SquadMember_Upgrades))
		{
			Widget = GetSquadMemberUpgradesMenu();
		} else
		{
			Widget = GetSquadMemberStatsMenu();
		}

		if (Widget) SubMenuSwitcher->SetActiveWidget(Widget);
	}
}

void UUpSquadMemberDisplayWidget::PopulateSubMenuData() const
{
	if (const auto SubMenu = GetSquadMemberStatsMenu())
	{
		if (Npc)
		{
			SubMenu->SetNpc(Npc);
		} else if (CharacterData.IsValid())
		{
			SubMenu->SetNpcTagId(CharacterData.TagId);
		}
	}
	
	if (const auto SubMenu = GetSquadMemberInventoryMenu())
	{
		if (Npc) SubMenu->SetNpc(Npc);
	}
}
