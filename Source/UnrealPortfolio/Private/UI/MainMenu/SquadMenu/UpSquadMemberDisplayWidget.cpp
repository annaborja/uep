// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UpGameInstance.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Components/PanelWidget.h"
#include "Tags/UiTags.h"
#include "UI/Basic/UpTabListWidget.h"
#include "UI/Basic/UpTabWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberInventoryMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberStatsMenuWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberDisplayWidget::PopulateNpcData(const FUpNpcData& InNpcData)
{
	NpcData = InNpcData;

	FUpReputationData PlayerReputationData;
			
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this); GameInstance && NpcData.IsValid())
	{
		PlayerReputationData = GameInstance->GetPlayerReputationData(NpcData.TagId);
	}
}

void UUpSquadMemberDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (const auto TabContentSwitcher = GetTabContentSwitcher())
	{
		if (const auto TabList = GetTabList(); TabList && TabList->GetTabCount() <= 0 && TabClass)
		{
			TabList->SetLinkedSwitcher(TabContentSwitcher);

			if (const auto Menu = GetSquadMemberStatsMenu())
			{
				TabList->RegisterTab(TAG_Menu_SquadMember_Stats.GetTag().GetTagName(), TabClass, Menu);
			}

			if (const auto Menu = GetSquadMemberInventoryMenu())
			{
				TabList->RegisterTab(TAG_Menu_SquadMember_Inventory.GetTag().GetTagName(), TabClass, Menu);
			}
		}
	}
}
