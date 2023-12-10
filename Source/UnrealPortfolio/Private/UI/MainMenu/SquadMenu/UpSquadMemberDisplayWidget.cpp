// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UpGameInstance.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Tags/UiTags.h"
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

void UUpSquadMemberDisplayWidget::SetActiveSubMenu(const FGameplayTag& SubMenuTag)
{
	if (const auto SubMenuSwitcher = GetSubMenuSwitcher())
	{
		UWidget* Widget;

		if (SubMenuTag.MatchesTagExact(TAG_Menu_SquadMember_Inventory))
		{
			Widget = GetSquadMemberInventoryMenu();
		} else
		{
			Widget = GetSquadMemberStatsMenu();
		}
		
		SubMenuSwitcher->SetActiveWidget(Widget);
	}
}
