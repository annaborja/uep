// Copyright AB. All Rights Reserved.

#include "UI/Basic/UpTabListWidget.h"

#include "UI/UpHud.h"
#include "UI/Basic/UpTabWidget.h"

void UUpTabListWidget::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);

	if (const auto MainMenuTab = Cast<UUpTabWidget>(TabButton); MainMenuTab && MenuTabDataTable)
	{
		TArray<FUpMenuTabData*> AllMenuTabDataRows;
		MenuTabDataTable->GetAllRows(TEXT("MenuTabData GetAllRows"), AllMenuTabDataRows);

		FUpMenuTabData MenuTabData;

		for (const auto MenuTabDataRow : AllMenuTabDataRows)
		{
			if (MenuTabDataRow->TagId.GetTagName() == TabNameID)
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

void UUpTabListWidget::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);

	if (const auto TabsContainer = GetTabsContainer())
	{
		TabsContainer->RemoveChild(TabButton);
	}
}
