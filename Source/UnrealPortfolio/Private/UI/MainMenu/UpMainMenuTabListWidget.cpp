// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/UpMainMenuTabListWidget.h"

#include "CommonButtonBase.h"
#include "Components/PanelWidget.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/UpMainMenuTabWidget.h"

void UUpMainMenuTabListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnTabButtonCreationDelegate.BindUFunction(this, FName("AddTabButton"));
	OnTabButtonRemovalDelegate.BindUFunction(this, FName("RemoveTabButton"));

	if (!OnTabButtonCreation.Contains(OnTabButtonCreationDelegate)) OnTabButtonCreation.Add(OnTabButtonCreationDelegate);
	if (!OnTabButtonRemoval.Contains(OnTabButtonRemovalDelegate)) OnTabButtonRemoval.Add(OnTabButtonRemovalDelegate);
}

void UUpMainMenuTabListWidget::AddTabButton(FName TabId, UCommonButtonBase* TabButton)
{
	if (const auto MainMenuTab = Cast<UUpMainMenuTabWidget>(TabButton); MainMenuTab && MenuTabDataTable)
	{
		TArray<FUpMenuTabData*> AllMenuTabDataRows;
		MenuTabDataTable->GetAllRows(TEXT("MenuTabData GetAllRows"), AllMenuTabDataRows);

		FUpMenuTabData MenuTabData;

		for (const auto MenuTabDataRow : AllMenuTabDataRows)
		{
			if (MenuTabDataRow->TagId.GetTagName() == TabId)
			{
				MenuTabData = *MenuTabDataRow;
				break;
			}
		}

		if (MenuTabData.IsValid())
		{
			MainMenuTab->SetLabel(MenuTabData.Label);

			if (const auto TabsContainer = GetTabsContainer())
			{
				TabsContainer->AddChild(MainMenuTab);
			}
		}
	}
}

void UUpMainMenuTabListWidget::RemoveTabButton(FName TabId, UCommonButtonBase* TabButton)
{
	if (const auto TabsContainer = GetTabsContainer())
	{
		TabsContainer->RemoveChild(TabButton);
	}
}
