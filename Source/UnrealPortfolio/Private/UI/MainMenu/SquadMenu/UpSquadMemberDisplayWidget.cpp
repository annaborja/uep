// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UpGameInstance.h"
#include "Tags/UiTags.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberSkillsMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberStatsMenuWidget.h"

void UUpSquadMemberDisplayWidget::SetNpc(AUpNpcCharacter* InNpc)
{
	Npc = InNpc;
	NpcData = Npc->GetNpcData();
	
	PopulateSubMenuData();
}

void UUpSquadMemberDisplayWidget::SetNpcData(const FUpNpcData InNpcData)
{
	Npc = nullptr;
	NpcData = InNpcData;
	
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
		} else if (SubMenuTag.MatchesTagExact(TAG_Menu_SquadMember_Skills))
		{
			Widget = GetSquadMemberSkillsMenu();
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
		} else if (NpcData.IsValid())
		{
			SubMenu->SetNpcTagId(NpcData.TagId);
		}
	}
	
	if (const auto SubMenu = GetSquadMemberInventoryMenu())
	{
		if (Npc)
		{
			SubMenu->SetNpc(Npc);
		} else if (NpcData.IsValid())
		{
			SubMenu->SetNpcTagId(NpcData.TagId);
		}
	}
}
